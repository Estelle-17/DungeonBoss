// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/DBInventoryWidget.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "UI/Inventory/DBInventoryBlockWidget.h"
#include "Interface/DBCharacterHUDInterface.h"
#include "GameData/DBItemSingleton.h"
#include "Item/DBItemObject.h"
#include "Player/DBPlayerBase.h"
#include "DBInventoryBlockWidget.h"
#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"

UDBInventoryWidget::UDBInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

//ItemSlot Setting
	//각 인벤토리의 칸을 TArray에 저장 및 초기화
	for (int index = 0; index < 80; index++)
	{
		FName InventoryBlockName = *FString::Printf(TEXT("InventoryBlock_%d"), index);
		UDBInventoryBlockWidget* InventoryBlockWidget = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(InventoryBlockName));
		ensure(InventoryBlockWidget);
		InventoryBlockWidget->SlotNumber = index;
		InventoryBlockWidget->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
		InventoryBlockWidget->OnEquipItems.AddUObject(this, &UDBInventoryWidget::SettingEquipItemForPlayer);
		InventoryBlockWidget->OnDivideItemSlider.AddUObject(this, &UDBInventoryWidget::SetSliderSetting);

		ItemSlots.Add(InventoryBlockWidget);
	}
	
	//UE_LOG(LogTemp, Log, TEXT("ItemSlot Count : %d"), ItemSlots.Num());

//CharacterEquip Setting
	EquipWeaponItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipWeaponImage")));
	ensure(EquipWeaponItem);
	EquipWeaponItem->SlotNumber = 80;
	EquipWeaponItem->bIsEquipSlot = true;
	//Delegate 설정
	EquipWeaponItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipWeaponItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipWeaponItem);

	EquipHeadItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipHeadImage")));
	ensure(EquipHeadItem);
	EquipHeadItem->SlotNumber = 81;
	EquipHeadItem->bIsEquipSlot = true;
	//Delegate 설정
	EquipHeadItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipHeadItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipHeadItem);

	EquipBodyItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipBodyImage")));
	ensure(EquipBodyItem);
	EquipBodyItem->SlotNumber = 82;
	EquipBodyItem->bIsEquipSlot = true;
	//Delegate 설정
	EquipBodyItem->OnDragSwapItems.AddUObject(this, &UDBInventoryWidget::DragSwapItems);
	EquipBodyItem->OnMoveEquipItemToEmptySlot.AddUObject(this, &UDBInventoryWidget::MoveEquipItemToEmptySlot);
	ItemSlots.Add(EquipBodyItem);

	EquipShoesItem = Cast<UDBInventoryBlockWidget>(GetWidgetFromName(TEXT("EquipShoesImage")));
	ensure(EquipShoesItem);
	EquipShoesItem->SlotNumber = 83;
	EquipShoesItem->bIsEquipSlot = true;
	//Delegate 설정
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

//ItemCountScrollBar Setting
	ItemCountScrollBarCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ItemCountScrollBarCanvas")));
	ensure(ItemCountScrollBarCanvas);

	CountSlider = Cast<USlider>(GetWidgetFromName(TEXT("CountSlider")));
	ensure(CountSlider);
	CountSlider->OnValueChanged.AddDynamic(this, &UDBInventoryWidget::ChangeCountText);

	MinCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MinCountText")));
	ensure(MinCountText);

	MaxCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxCountText")));
	ensure(MaxCountText);

	OKButton = Cast<UButton>(GetWidgetFromName(TEXT("OKButton")));
	ensure(OKButton);
	OKButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::DivideItem);

	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));
	ensure(CancelButton);
	CancelButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::CancelDivideItem);

	ItemCountScrollBarCanvas->SetVisibility(ESlateVisibility::Collapsed);
	DivideItemCount = 1;

//AddItem
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
	AddCountableItem(TEXT("ARMORED_BOSS_01"), 10, true);
	AddCountableItem(TEXT("ARMORED_BOSS_01"), 5, true);
	AddCountableItem(TEXT("ARMORED_BOSS_02"), 5, true);

}

void UDBInventoryWidget::AddEquipItem(FName ItemID)
{
	//UDBEquipItemData* EquipItemData = UDBItemSingleton::Get().AddEquipItem(ItemID);
	UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());
	ItemObject->MakeEquipItemData(ItemID);

	const UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	if (EquipItemData)
	{	
		//아이템을 인벤토리에 넣어줌
		int CanInputItemSlotIndex = CheckEmptyItemSlot();
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemSlots[CanInputItemSlotIndex]->GetName());
		ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
		ItemSlots[CanInputItemSlotIndex]->ItemSlotType = EItemSlotType::Item;
	}
}

void UDBInventoryWidget::AddCountableItem(FName ItemID, int ItemCount, bool bIsItemOverlap)
{
	//UDBCountableItemData* CountableItemData = UDBItemSingleton::Get().AddCountableItem(ItemID, ItemCount);
	//이미 인벤토리에 아이템이 존재하는지 확인
	//CheckCountableItem이 -1일 경우 인벤토리에 아이템이 없다는 뜻
	int CheckCountableItem = FindCountableItem(ItemID);

	if (CheckCountableItem != -1 && bIsItemOverlap)
	{
		//아이템이 존재할 시 갯수 변경
		ItemSlots[CheckCountableItem]->GetItemObjectData()->SetItemCount(ItemCount);
	}
	else
	{
		UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());

		if (ItemObject)
		{
			ItemObject->MakeCountableItemData(ItemID, ItemCount);

			//겹칠 수 있는 새로운 아이템을 인벤토리에 넣어줌
			//CountableItems.Emplace(ItemID, ItemObject);

			//아이템을 인벤토리에 넣어줌
			int CanInputItemSlotIndex = CheckEmptyItemSlot();
			ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
			ItemSlots[CanInputItemSlotIndex]->ItemSlotType = EItemSlotType::Item;
		}
	}
	//아이템 갯수 저장
	UDBItemSingleton::Get().SetCountableItemObject(ItemID, ItemCount);
}

void UDBInventoryWidget::SettingEquipItemForPlayer(UDBItemObject* ItemObject, int FromSlotNumber, EEquipItemType FromEquipItemType, EItemSlotType FromItemSlotType)
{
	UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	//이전에 장착된 아이템이 있을 경우 제거
	switch (FromEquipItemType)
	{
		case EEquipItemType::Weapon:	//Weapon
			//전에 장착하고 있는 아이템을 인벤토리로 이동
			if (CharacterEquipStats.WeaponEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 80, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 80, EItemSlotType::None);
			}
			//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
			CharacterEquipStats.WeaponEquipItem = ItemObject;

			break;
		case EEquipItemType::Head:	//Head
			//전에 장착하고 있는 아이템을 인벤토리로 이동
			if (CharacterEquipStats.HeadEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 81, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 81, EItemSlotType::None);
			}
			//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
			CharacterEquipStats.HeadEquipItem = ItemObject;

			break;
		case EEquipItemType::Body:	//Body
			//전에 장착하고 있는 아이템을 인벤토리로 이동
			if (CharacterEquipStats.BodyEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 82, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 82, EItemSlotType::None);
			}
			//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
			CharacterEquipStats.BodyEquipItem = ItemObject;

			break;
		case EEquipItemType::Shoes:	//Shoes
			//전에 장착하고 있는 아이템을 인벤토리로 이동
			if (CharacterEquipStats.ShoesEquipItem)
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 83, EItemSlotType::Item);
			}
			else
			{
				DragSwapItems(FromSlotNumber, FromItemSlotType, 83, EItemSlotType::None);
			}
			//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
			CharacterEquipStats.ShoesEquipItem = ItemObject;

			break;
	}

	//플레이어한테 아이템 스탯 적용
	SetCharacterItemStat(ItemObject->GetEquipItemData()->GetEquipCharacterStat(), ItemObject->GetEquipItemData()->GetEquipItemStat().ItemType);
}

void UDBInventoryWidget::DragSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, int ToItemNumber, EItemSlotType ToItemSlotType)
{
	UDBItemObject* ItemObject = ItemSlots[FromItemNumber]->GetItemObjectData();
	
	//만약 FromItem이 CountableItem일때 장착 칸으로 이동할 경우 이동X
	if (ItemSlots[FromItemNumber]->GetItemObjectData()->bIsCountableItem && ToItemNumber >= 80)
	{
		ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
		return;
	}

	if (ToItemSlotType == EItemSlotType::None)
	{
		ItemSlots[ToItemNumber]->SetItemSetting(ItemObject);
		ItemSlots[ToItemNumber]->ItemSlotType = FromItemSlotType;

		ItemSlots[FromItemNumber]->SetTranslucnetImageDisable();
		ItemSlots[FromItemNumber]->ResetInventorySlot();
	}
	else
	{
		//두 아이템이 같은 종류의 아이템일 경우 합치기
		if (ItemSlots[FromItemNumber]->GetItemObjectData()->ItemID == ItemSlots[ToItemNumber]->GetItemObjectData()->ItemID)
		{
			ItemSlots[ToItemNumber]->GetItemObjectData()->SetItemCount(ItemSlots[FromItemNumber]->GetItemObjectData()->GetItemCount());

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
	}

	//장비 창으로 이동 시 장비 스탯 적용
	if (FromItemNumber >= 80)
	{
		//아이템을 좌클릭으로 집고 이동하여 빈 슬롯이 된 경우
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
		//아이템을 좌클릭으로 집고 이동하여 빈 슬롯이 된 경우
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
			//아이템이 존재하는지 확인 후 Swap진행
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
			//아이템이 존재하는지 확인 후 Swap진행
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
			//아이템이 존재하는지 확인 후 Swap진행
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
			//아이템이 존재하는지 확인 후 Swap진행
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

int UDBInventoryWidget::FindCountableItem(FName NewItemID)
{
	for (int index = 0; index < 80; index++)
	{
		UDBItemObject* ItemObject = ItemSlots[index]->GetItemObjectData();
		if (ItemObject)
		{
			if (ItemObject->ItemID == NewItemID)
			{
				return index;
			}
		}
	}

	return -1;
}

void UDBInventoryWidget::RemoveCountableItem(FName TargetItemID, int ItemCount)
{
	int CurrentItemCount = ItemCount;
	for (int index = 0; index < 80; index++)
	{
		UDBItemObject* ItemObject = ItemSlots[index]->GetItemObjectData();
		if (ItemObject)
		{
			//재료의 ID값이 같은지 확인
			if (ItemObject->ItemID == TargetItemID)
			{
				//아이템의 수량이 제거해야할 수량과 같거나 적은 경우
				if (ItemObject->GetItemCount() <= CurrentItemCount)
				{
					//아이템 제거 및 제거해야할 수량 감소
					CurrentItemCount -= ItemObject->GetItemCount();
					UDBItemSingleton::Get().SetCountableItemObject(TargetItemID, -ItemObject->GetItemCount());

					//가비지 컬렉터에 UDBItemObject데이터 등록
					ItemSlots[index]->GetItemObjectData()->MarkAsGarbage();
					ItemSlots[index]->ResetInventorySlot();
				}
				else
				{
					//아이템 수량 감소
					ItemObject->SetItemCount(-CurrentItemCount);
					UDBItemSingleton::Get().SetCountableItemObject(TargetItemID, -CurrentItemCount);

					return;
				}
			}
		}
		if (CurrentItemCount <= 0)
		{
			return;
		}
	}
}

void UDBInventoryWidget::SettingCharacterEquipStats(UDBItemObject* ItemObject, int ItemSlotNumber)
{
	//이전에 장착된 아이템이 있을 경우 제거
	switch (ItemSlotNumber)
	{
	case 80:	//Weapon
		//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.WeaponEquipItem = ItemObject;
		//플레이어한테 아이템 스탯 적용
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
		//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.HeadEquipItem = ItemObject;
		//플레이어한테 아이템 스탯 적용
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
		//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.BodyEquipItem = ItemObject;
		//플레이어한테 아이템 스탯 적용
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
		//스탯 적용을 위해 새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.ShoesEquipItem = ItemObject;
		//플레이어한테 아이템 스탯 적용
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

#pragma region ItemCountSliderBar Setting

FReply UDBInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::Escape)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Inventory"));

		ADBPlayerController* PlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());
		//PlayerContorller에서 ItemDragVisualWidget가져오기
		if (PlayerController)
		{
			PlayerController->CollapseWidget(TEXT("Inventory"));
		}
	}

	return Reply.NativeReply;
}

#pragma endregion

#pragma region ItemCountSliderBar Setting

void UDBInventoryWidget::SetSliderSetting(UDBItemObject* ItemObject, int MaxCount)
{
	ItemCountScrollBarCanvas->SetVisibility(ESlateVisibility::Visible);

	//현재 아이템의 갯수 저장 및 현재 나눌 아이템의 데이터 저장
	CountSlider->SetMinValue(1);
	CountSlider->SetMaxValue(MaxCount - 1);
	CountSlider->SetValue(1);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Min : %f, Max : %f"), CountSlider->GetMinValue(), CountSlider->GetMaxValue()));

	//눈에 보이는 숫자 설정
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), 1.0f)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CountSlider->GetMaxValue())));

	CurrentItemObject = ItemObject;
}

void UDBInventoryWidget::DivideItem()
{
	//나누어진 갯수만큼 아이템을 둘로 나누어줌
	CurrentItemObject->SetItemCount(int(-DivideItemCount));

	AddCountableItem(CurrentItemObject->ItemID, int(DivideItemCount), false);

	ItemCountScrollBarCanvas->SetVisibility(ESlateVisibility::Collapsed);
}

void UDBInventoryWidget::CancelDivideItem()
{
	ItemCountScrollBarCanvas->SetVisibility(ESlateVisibility::Collapsed);
}

void UDBInventoryWidget::ChangeCountText(float Value)
{
	DivideItemCount = CountSlider->GetValue();
	DivideItemCount = round(DivideItemCount);
	CountSlider->SetValue(DivideItemCount);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Value : %.0f"), DivideItemCount));

	//눈에 보이는 숫자 설정
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DivideItemCount)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), (CountSlider->GetMaxValue() - DivideItemCount) + 1)));
}

#pragma endregion
