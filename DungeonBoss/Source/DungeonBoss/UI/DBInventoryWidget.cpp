// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DBInventoryWidget.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/DBInventoryBlockWidget.h"
#include "Interface/DBCharacterHUDInterface.h"
#include "GameData/DBItemSingleton.h"
#include "Item/DBItemObject.h"
#include "Player/DBPlayerBase.h"
#include "DBInventoryBlockWidget.h"
#include "DungeonBoss.h"

UDBInventoryWidget::UDBInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

//ItemSlot Setting
	//�� �κ��丮�� ĭ�� TArray�� ���� �� �ʱ�ȭ
	for (int index = 0; index < 80; index++)
	{
		FName InventoryBlockName = *FString::Printf(TEXT("InventoryBlock_%d"), index);
		UDBInventoryBlockWidget* InventoryBlockWidget = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(InventoryBlockName));
		ensure(InventoryBlockWidget);
		InventoryBlockWidget->SlotNumber = index;
		InventoryBlockWidget->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
		InventoryBlockWidget->OnEquipItems.AddUObject(this, &UDBInventoryWidget::SettingEquipItemForPlayer);

		ItemSlots.Add(InventoryBlockWidget);
	}
	
	//UE_LOG(LogTemp, Log, TEXT("ItemSlot Count : %d"), ItemSlots.Num());

//CharacterEquip Setting
	EquipWeaponItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipWeaponImage")));
	ensure(EquipWeaponItem);
	EquipWeaponItem->SlotNumber = 80;
	EquipWeaponItem->bIsEquipSlot = true;
	//Delegate ����
	EquipWeaponItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipWeaponItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipWeaponItem);

	EquipHeadItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipHeadImage")));
	ensure(EquipHeadItem);
	EquipHeadItem->SlotNumber = 81;
	EquipHeadItem->bIsEquipSlot = true;
	//Delegate ����
	EquipHeadItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipHeadItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipHeadItem);

	EquipBodyItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipBodyImage")));
	ensure(EquipBodyItem);
	EquipBodyItem->SlotNumber = 82;
	EquipBodyItem->bIsEquipSlot = true;
	//Delegate ����
	EquipBodyItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipBodyItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipBodyItem);

	EquipShoesItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipShoesImage")));
	ensure(EquipShoesItem);
	EquipShoesItem->SlotNumber = 83;
	EquipShoesItem->bIsEquipSlot = true;
	//Delegate ����
	EquipShoesItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipShoesItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipShoesItem);

//Player Setting
	IDBCharacterHUDInterface* HUDPawn = Cast<IDBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupInventoryWidget(this);
	}

	ADBPlayerBase* PlayerPawn = Cast<ADBPlayerBase>(GetOwningPlayerPawn());
	if (PlayerPawn)
	{
		CharacterStat = PlayerPawn->GetCharacterStatComponent();
	}

	AddEquipItem(TEXT("WEAPON_001"));
	AddEquipItem(TEXT("WEAPON_001"));
	AddEquipItem(TEXT("HEAD_001"));
	AddEquipItem(TEXT("HEAD_001"));
	AddEquipItem(TEXT("BODY_001"));
	AddEquipItem(TEXT("BODY_001"));
	AddEquipItem(TEXT("BODY_001"));
	AddEquipItem(TEXT("SHOES_001"));
	AddEquipItem(TEXT("SHOES_001"));
	AddEquipItem(TEXT("SHOES_001"));
	AddEquipItem(TEXT("SHOES_001"));
	AddCountableItem(TEXT("BOSS_01_01"), 10);
	AddCountableItem(TEXT("BOSS_01_01"), 5);
	AddCountableItem(TEXT("BOSS_01_02"), 5);
}

void UDBInventoryWidget::AddEquipItem(FName ItemID)
{
	//UDBEquipItemData* EquipItemData = UDBItemSingleton::Get().AddEquipItem(ItemID);
	UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());
	ItemObject->MakeEquipItemData(ItemID);

	const UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	if (EquipItemData)
	{	
		//�������� �κ��丮�� �־���
		int CanInputItemSlotIndex = CheckEmptyItemSlot();
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemSlots[CanInputItemSlotIndex]->GetName());
		ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
		ItemSlots[CanInputItemSlotIndex]->ItemSlotType = EItemSlotType::Item;
	}
}

void UDBInventoryWidget::AddCountableItem(FName ItemID, int ItemCount)
{
	//UDBCountableItemData* CountableItemData = UDBItemSingleton::Get().AddCountableItem(ItemID, ItemCount);
	//�̹� �κ��丮�� �������� �����ϴ��� Ȯ��
	if (CountableItems.Contains(ItemID))
	{;
		//�������� ������ �� �ҷ��� ��� �����ؾ���
		CountableItems[ItemID]->SetItemCount(ItemCount);
	}
	else
	{
		UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());

		if (ItemObject)
		{
			ItemObject->MakeCountableItemData(ItemID, ItemCount);
			//��ĥ �� �ִ� ���ο� �������� �κ��丮�� �־���
			CountableItems.Emplace(ItemID, ItemObject);

			//�������� �κ��丮�� �־���
			int CanInputItemSlotIndex = CheckEmptyItemSlot();
			ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
			ItemSlots[CanInputItemSlotIndex]->ItemSlotType = EItemSlotType::Item;
		}
	}
}

void UDBInventoryWidget::SettingEquipItemForPlayer(UDBItemObject* ItemObject, int FromSlotNumber, EEquipItemType FromEquipItemType, EItemSlotType FromItemSlotType)
{
	UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	//������ ������ �������� ���� ��� ����
	switch (FromEquipItemType)
	{
		case EEquipItemType::Weapon:	//Weapon
			//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
			if (CharacterEquipStats.WeaponEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 80, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 80, EItemSlotType::None);
			}
			//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
			CharacterEquipStats.WeaponEquipItem = ItemObject;

			break;
		case EEquipItemType::Head:	//Head
			//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
			if (CharacterEquipStats.HeadEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 81, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 81, EItemSlotType::None);
			}
			//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
			CharacterEquipStats.HeadEquipItem = ItemObject;

			break;
		case EEquipItemType::Body:	//Body
			//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
			if (CharacterEquipStats.BodyEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 82, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 82, EItemSlotType::None);
			}
			//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
			CharacterEquipStats.BodyEquipItem = ItemObject;

			break;
		case EEquipItemType::Shoes:	//Shoes
			//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
			if (CharacterEquipStats.ShoesEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 83, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 83, EItemSlotType::None);
			}
			//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
			CharacterEquipStats.ShoesEquipItem = ItemObject;

			break;
	}

	//�÷��̾����� ������ ���� ����
	SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), ItemObject->GetEquipItemData()->GetEquipItemStat().ItemType);
}

void UDBInventoryWidget::DragSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, int ToItemNumber, EItemSlotType ToItemSlotType)
{
	UDBItemObject* ItemObject = ItemSlots[FromItemNumber]->GetItemObjectData();
	
	if (ToItemSlotType == EItemSlotType::None)
	{
		ItemSlots[ToItemNumber]->SetItemSetting(ItemObject);
		ItemSlots[ToItemNumber]->ItemSlotType = FromItemSlotType;

		ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
		ItemSlots[FromItemNumber]->ResetInventorySlot();
	}
	else
	{
		ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
		ItemSlots[FromItemNumber]->SetItemSetting(ItemSlots[ToItemNumber]->GetItemObjectData());
		ItemSlots[FromItemNumber]->ItemSlotType = ToItemSlotType;

		ItemSlots[ToItemNumber]->SetItemSetting(ItemObject);
		ItemSlots[ToItemNumber]->ItemSlotType = FromItemSlotType;
	}

	//��� â���� �̵� �� ��� ���� ����
	if (FromItemNumber >= 80)
	{
		//�������� ��Ŭ������ ���� �̵��Ͽ� �� ������ �� ���
		if (ItemSlots[FromItemNumber]->ItemSlotType == EItemSlotType::None)
		{
			SettingCharacterEquipStats(nullptr, FromItemNumber);
		}
		else
		{
			if (ItemSlots[FromItemNumber]->GetItemObjectData())
			{
				UDBEquipItemData* EquipItemData = ItemSlots[FromItemNumber]->GetItemObjectData()->GetEquipItemData();
				SetCharacterItemStat(EquipItemData->GetEquipCharacterStat(), EquipItemData->GetEquipItemStat().ItemType);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("No Item %d"), FromItemNumber));
			}
		}
	}
	if (ToItemNumber >= 80)
	{
		//�������� ��Ŭ������ ���� �̵��Ͽ� �� ������ �� ���
		if (ItemSlots[ToItemNumber]->ItemSlotType == EItemSlotType::None)
		{
			SettingCharacterEquipStats(nullptr, ToItemNumber);
		}
		else
		{
			if (ItemSlots[ToItemNumber]->GetItemObjectData())
			{
				UDBEquipItemData* EquipItemData = ItemSlots[ToItemNumber]->GetItemObjectData()->GetEquipItemData();
				SetCharacterItemStat(EquipItemData->GetEquipCharacterStat(), EquipItemData->GetEquipItemStat().ItemType);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("No Item %d"), ToItemNumber));
			}
		}
	}
}

void UDBInventoryWidget::EquipSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, EEquipItemType EquipItemType, EItemSlotType ToItemSlotType)
{
	UDBItemObject* ItemObject = ItemSlots[FromItemNumber]->GetItemObjectData();

	switch (EquipItemType)
	{
		case EEquipItemType::Weapon:	//Weapon
			//�������� �����ϴ��� Ȯ�� �� Swap����
			if (ToItemSlotType == EItemSlotType::None)
			{
				EquipWeaponItem->SetItemSetting(ItemObject);
				EquipWeaponItem->ItemSlotType = FromItemSlotType;

				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->ResetInventorySlot();
			}
			else
			{
				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->SetItemSetting(EquipWeaponItem->GetItemObjectData());
				ItemSlots[FromItemNumber]->ItemSlotType = ToItemSlotType;

				EquipWeaponItem->SetItemSetting(ItemObject);
				EquipWeaponItem->ItemSlotType = FromItemSlotType;
			}
			break;
		case EEquipItemType::Head:	//Head
			//�������� �����ϴ��� Ȯ�� �� Swap����
			if (ToItemSlotType == EItemSlotType::None)
			{
				EquipHeadItem->SetItemSetting(ItemObject);
				EquipHeadItem->ItemSlotType = FromItemSlotType;

				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->ResetInventorySlot();
			}
			else
			{
				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->SetItemSetting(EquipHeadItem->GetItemObjectData());
				ItemSlots[FromItemNumber]->ItemSlotType = ToItemSlotType;

				EquipHeadItem->SetItemSetting(ItemObject);
				EquipHeadItem->ItemSlotType = FromItemSlotType;
			}
			break;
		case EEquipItemType::Body:	//Body
			//�������� �����ϴ��� Ȯ�� �� Swap����
			if (ToItemSlotType == EItemSlotType::None)
			{
				EquipBodyItem->SetItemSetting(ItemObject);
				EquipBodyItem->ItemSlotType = FromItemSlotType;

				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->ResetInventorySlot();
			}
			else
			{
				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->SetItemSetting(EquipBodyItem->GetItemObjectData());
				ItemSlots[FromItemNumber]->ItemSlotType = ToItemSlotType;

				EquipBodyItem->SetItemSetting(ItemObject);
				EquipBodyItem->ItemSlotType = FromItemSlotType;
			}
			break;
		case EEquipItemType::Shoes:	//Shoes
			//�������� �����ϴ��� Ȯ�� �� Swap����
			if (ToItemSlotType == EItemSlotType::None)
			{
				EquipShoesItem->SetItemSetting(ItemObject);
				EquipShoesItem->ItemSlotType = FromItemSlotType;

				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->ResetInventorySlot();
			}
			else
			{
				ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
				ItemSlots[FromItemNumber]->SetItemSetting(EquipShoesItem->GetItemObjectData());
				ItemSlots[FromItemNumber]->ItemSlotType = ToItemSlotType;

				EquipShoesItem->SetItemSetting(ItemObject);
				EquipShoesItem->ItemSlotType = FromItemSlotType;
			}
			break;
	}
}

void UDBInventoryWidget::SetCharacterItemStat(FDBCharacterStat NewEquipStat, int32 ItemType)
{
	if (CharacterStat)
	{
		CharacterStat->SetModifierStat(NewEquipStat, ItemType);
	}
}

void UDBInventoryWidget::MoveEquipItemToEmptySlot(UDBItemObject* ItemObject, int ItemSlotNumber)
{
	for (int index = 0; index < 80; index++)
	{
		if (ItemSlots[index]->ItemSlotType == EItemSlotType::None)
		{
			ItemSlots[index]->SetItemSetting(ItemObject);
			ItemSlots[index]->ItemSlotType = EItemSlotType::Item;

			ItemSlots[ItemSlotNumber]->SetTranslucnetImageDisable();
			ItemSlots[ItemSlotNumber]->ResetInventorySlot();

			SettingCharacterEquipStats(nullptr, ItemSlotNumber);
			break;
		}
	}
}

int UDBInventoryWidget::CheckEmptyItemSlot()
{
	for (int index = 0; index < 80; index++)
	{
		if (ItemSlots[index]->ItemSlotType == EItemSlotType::None)
		{
			return index;
		}
	}
	
	return -1;
}

void UDBInventoryWidget::SettingCharacterEquipStats(UDBItemObject* ItemObject, int ItemSlotNumber)
{
	//������ ������ �������� ���� ��� ����
	switch (ItemSlotNumber)
	{
	case 80:	//Weapon
		//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.WeaponEquipItem = ItemObject;
		//�÷��̾����� ������ ���� ����
		if (ItemObject == nullptr)
		{
			FDBCharacterStat EmptyCharacterStat;
			SetCharacterItemStat(EmptyCharacterStat, 0);
		}
		else
		{
			SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), 0);
		}
		break;
	case 81:	//Head
		//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.HeadEquipItem = ItemObject;
		//�÷��̾����� ������ ���� ����
		if (ItemObject == nullptr)
		{
			FDBCharacterStat EmptyCharacterStat;
			SetCharacterItemStat(EmptyCharacterStat, 1);
		}
		else
		{
			SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), 1);
		}
		break;
	case 82:	//Body
		//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.BodyEquipItem = ItemObject;
		//�÷��̾����� ������ ���� ����
		if (ItemObject == nullptr)
		{
			FDBCharacterStat EmptyCharacterStat;
			SetCharacterItemStat(EmptyCharacterStat, 2);
		}
		else
		{
			SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), 2);
		}
		break;
	case 83:	//Shoes
		//���� ������ ���� ���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.ShoesEquipItem = ItemObject;
		//�÷��̾����� ������ ���� ����
		if (ItemObject == nullptr)
		{
			FDBCharacterStat EmptyCharacterStat;
			SetCharacterItemStat(EmptyCharacterStat, 3);
		}
		else
		{
			SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), 3);
		}
		break;
	}
}

void UDBInventoryWidget::InventoryItemClicked(UObject* Item)
{
	UE_LOG(LogTemp, Log, TEXT("Is Clicked! : %s"), *Item->GetFName().ToString())
}
