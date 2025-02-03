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
	//�� �κ��丮�� ĭ�� TArray�� ����
	for (int index = 0; index < 80; index++)
	{
		FName InventoryBlockName = *FString::Printf(TEXT("InventoryBlock_%d"), index);
		UDBInventoryBlockWidget* InventoryBlockWidget = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(InventoryBlockName));
		ensure(InventoryBlockWidget);
		ItemSlots.Add(InventoryBlockWidget);
	}
	CanInputItemSlotIndex = 0;
	
	//UE_LOG(LogTemp, Log, TEXT("ItemSlot Count : %d"), ItemSlots.Num());

//CharacterEquip Setting
	HeadImage = Cast<UImage>(GetWidgetFromName(TEXT("EquipHeadImage")));
	ensure(HeadImage);

	BodyImage = Cast<UImage>(GetWidgetFromName(TEXT("EquipBodyImage")));
	ensure(BodyImage);

	ShoesImage = Cast<UImage>(GetWidgetFromName(TEXT("EquipShoesImage")));
	ensure(ShoesImage);

	WeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("EquipWeaponImage")));
	ensure(WeaponImage);

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
}

void UDBInventoryWidget::AddEquipItem(FName ItemID)
{
	//UDBEquipItemData* EquipItemData = UDBItemSingleton::Get().AddEquipItem(ItemID);
	UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());
	ItemObject->MakeEquipItemData(ItemID);

	const UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	//��� �������� ������ �� ������ �Լ��� Delegate�� �߰�
	ItemObject->OnPlayerSetEquipItem.AddUObject(this, &UDBInventoryWidget::SettingEquipItemForPlayer);

	if (EquipItemData)
	{	
		//�������� �κ��丮�� �־���
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemSlots[CanInputItemSlotIndex]->GetName());
		ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
		CanInputItemSlotIndex++;
	}
}

void UDBInventoryWidget::AddCountableItem(FName ItemID, int32 ItemCount)
{
	//UDBCountableItemData* CountableItemData = UDBItemSingleton::Get().AddCountableItem(ItemID, ItemCount);
	//�̹� �κ��丮�� �������� �����ϴ��� Ȯ��
	if (UDBItemSingleton::Get().IsContainCountableItem(ItemID))
	{
		UDBItemObject* ItemObject = UDBItemSingleton::Get().GetCountableItemObject(ItemID);
		//�������� ������ �� �ҷ��� ��� �����ؾ���
		ItemObject->SetItemCount(ItemCount);
	}
	else
	{
		UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());
		ItemObject->MakeCountableItemData(ItemID, ItemCount);

		//��ĥ �� �ִ� ���ο� �������� �κ��丮�� �־���

	}
}

void UDBInventoryWidget::SettingEquipItemForPlayer(UDBItemObject* ItemObject)
{
	UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	//������ ������ �������� ���� ��� ����
	switch (EquipItemData->GetEquipItemStat().ItemType)
	{
	case 0:	//Weapon
		//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
		if (CharacterEquipStats.WeaponEquipItem)
		{
			
		}
		//���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.WeaponEquipItem = ItemObject;

		//������ �������� �̹����� ĳ���� ������ ����UI�� �־��ֱ�
		WeaponImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		WeaponImage->SetDesiredSizeOverride(WeaponImage->GetBrush().ImageSize);

		break;
	case 1:	//Head
		//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
		if (CharacterEquipStats.HeadEquipItem)
		{
			
		}
		//���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.HeadEquipItem = ItemObject;

		//������ �������� �̹����� ĳ���� ������ ����UI�� �־��ֱ�
		HeadImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		HeadImage->SetDesiredSizeOverride(HeadImage->GetBrush().ImageSize);
		break;
	case 2:	//Body
		//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
		if (CharacterEquipStats.BodyEquipItem)
		{
			
		}
		//���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.BodyEquipItem = ItemObject;

		//������ �������� �̹����� ĳ���� ������ ����UI�� �־��ֱ�
		BodyImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		BodyImage->SetDesiredSizeOverride(BodyImage->GetBrush().ImageSize);
		break;
	case 3:	//Shoes
		//���� �����ϰ� �ִ� �������� �κ��丮�� �̵�
		if (CharacterEquipStats.ShoesEquipItem)
		{
			
		}
		//���Ӱ� �����ϰ� �� ������ �ּ� ����
		CharacterEquipStats.ShoesEquipItem = ItemObject;

		//������ �������� �̹����� ĳ���� ������ ����UI�� �־��ֱ�
		ShoesImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		ShoesImage->SetDesiredSizeOverride(ShoesImage->GetBrush().ImageSize);
		break;
	}
	UE_LOG(LogTemp, Log, TEXT("EquipItemSetting On"))

	//�÷��̾����� ������ ���� ����
	if (CharacterStat)
	{
		CharacterStat->SetModifierStat(EquipItemData->GetEquipCharacterStat(), EquipItemData->GetEquipItemStat().ItemType);
	}
}

void UDBInventoryWidget::InventoryItemClicked(UObject* Item)
{
	UE_LOG(LogTemp, Log, TEXT("Is Clicked! : %s"), *Item->GetFName().ToString())
}
