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
	//각 인벤토리의 칸을 TArray에 저장
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

	//장비 아이템을 장착할 때 실행할 함수를 Delegate에 추가
	ItemObject->OnPlayerSetEquipItem.AddUObject(this, &UDBInventoryWidget::SettingEquipItemForPlayer);

	if (EquipItemData)
	{	
		//아이템을 인벤토리에 넣어줌
		UE_LOG(LogTemp, Log, TEXT("%s"), *ItemSlots[CanInputItemSlotIndex]->GetName());
		ItemSlots[CanInputItemSlotIndex]->SetItemSetting(ItemObject);
		CanInputItemSlotIndex++;
	}
}

void UDBInventoryWidget::AddCountableItem(FName ItemID, int32 ItemCount)
{
	//UDBCountableItemData* CountableItemData = UDBItemSingleton::Get().AddCountableItem(ItemID, ItemCount);
	//이미 인벤토리에 아이템이 존재하는지 확인
	if (UDBItemSingleton::Get().IsContainCountableItem(ItemID))
	{
		UDBItemObject* ItemObject = UDBItemSingleton::Get().GetCountableItemObject(ItemID);
		//아이템이 존재할 시 불러올 방법 제작해야함
		ItemObject->SetItemCount(ItemCount);
	}
	else
	{
		UDBItemObject* ItemObject = NewObject<UDBItemObject>(this, UDBItemObject::StaticClass());
		ItemObject->MakeCountableItemData(ItemID, ItemCount);

		//겹칠 수 있는 새로운 아이템을 인벤토리에 넣어줌

	}
}

void UDBInventoryWidget::SettingEquipItemForPlayer(UDBItemObject* ItemObject)
{
	UDBEquipItemData* EquipItemData = ItemObject->GetEquipItemData();

	//이전에 장착된 아이템이 있을 경우 제거
	switch (EquipItemData->GetEquipItemStat().ItemType)
	{
	case 0:	//Weapon
		//전에 장착하고 있는 아이템을 인벤토리로 이동
		if (CharacterEquipStats.WeaponEquipItem)
		{
			
		}
		//새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.WeaponEquipItem = ItemObject;

		//장착한 아이템의 이미지를 캐릭터 아이템 장착UI에 넣어주기
		WeaponImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		WeaponImage->SetDesiredSizeOverride(WeaponImage->GetBrush().ImageSize);

		break;
	case 1:	//Head
		//전에 장착하고 있는 아이템을 인벤토리로 이동
		if (CharacterEquipStats.HeadEquipItem)
		{
			
		}
		//새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.HeadEquipItem = ItemObject;

		//장착한 아이템의 이미지를 캐릭터 아이템 장착UI에 넣어주기
		HeadImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		HeadImage->SetDesiredSizeOverride(HeadImage->GetBrush().ImageSize);
		break;
	case 2:	//Body
		//전에 장착하고 있는 아이템을 인벤토리로 이동
		if (CharacterEquipStats.BodyEquipItem)
		{
			
		}
		//새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.BodyEquipItem = ItemObject;

		//장착한 아이템의 이미지를 캐릭터 아이템 장착UI에 넣어주기
		BodyImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		BodyImage->SetDesiredSizeOverride(BodyImage->GetBrush().ImageSize);
		break;
	case 3:	//Shoes
		//전에 장착하고 있는 아이템을 인벤토리로 이동
		if (CharacterEquipStats.ShoesEquipItem)
		{
			
		}
		//새롭게 장착하게 된 아이템 주소 저장
		CharacterEquipStats.ShoesEquipItem = ItemObject;

		//장착한 아이템의 이미지를 캐릭터 아이템 장착UI에 넣어주기
		ShoesImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		ShoesImage->SetDesiredSizeOverride(ShoesImage->GetBrush().ImageSize);
		break;
	}
	UE_LOG(LogTemp, Log, TEXT("EquipItemSetting On"))

	//플레이어한테 아이템 스탯 적용
	if (CharacterStat)
	{
		CharacterStat->SetModifierStat(EquipItemData->GetEquipCharacterStat(), EquipItemData->GetEquipItemStat().ItemType);
	}
}

void UDBInventoryWidget::InventoryItemClicked(UObject* Item)
{
	UE_LOG(LogTemp, Log, TEXT("Is Clicked! : %s"), *Item->GetFName().ToString())
}
