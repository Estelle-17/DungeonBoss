// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DBInventoryWidget.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "UI/DBInventoryBlockWidget.h"
#include "Interface/DBCharacterHUDInterface.h"
#include "GameData/DBItemSingleton.h"
#include "DungeonBoss.h"

UDBInventoryWidget::UDBInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

//TileView Setting
	WeaponItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("WeaponItemBlocks")));
	ensure(WeaponItemBlocks);

	HeadItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("HeadItemBlocks")));
	ensure(HeadItemBlocks);
	HeadItemBlocks->SetVisibility(ESlateVisibility::Collapsed);

	BodyItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("BodyItemBlocks")));
	ensure(BodyItemBlocks);
	BodyItemBlocks->SetVisibility(ESlateVisibility::Collapsed);

	ShoesItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("ShoesItemBlocks")));
	ensure(ShoesItemBlocks);
	ShoesItemBlocks->SetVisibility(ESlateVisibility::Collapsed);

	CountableItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("CountableItemBlocks")));
	ensure(CountableItemBlocks);
	CountableItemBlocks->SetVisibility(ESlateVisibility::Collapsed);

//Button Setting
	WeaponItemButton = Cast<UButton>(GetWidgetFromName(TEXT("WeaponItemButton")));
	ensure(WeaponItemBlocks);
	WeaponItemButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::WeaponItemButtonCallback);

	HeadItemButton = Cast<UButton>(GetWidgetFromName(TEXT("HeadItemButton")));
	ensure(HeadItemButton);
	HeadItemButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::HeadItemButtonCallback);

	BodyItemButton = Cast<UButton>(GetWidgetFromName(TEXT("BodyItemButton")));
	ensure(BodyItemButton);
	BodyItemButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::BodyItemButtonCallback);

	ShoesItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ShoesItemButton")));
	ensure(ShoesItemButton);
	ShoesItemButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::ShoesItemButtonCallback);

	CountableItemButton = Cast<UButton>(GetWidgetFromName(TEXT("CountableItemButton")));
	ensure(CountableItemButton);
	CountableItemButton->OnClicked.AddDynamic(this, &UDBInventoryWidget::CountableItemButtonCallback);

//Player Setting
	IDBCharacterHUDInterface* HUDPawn = Cast<IDBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupInventoryWidget(this);
	}
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
	UDBEquipItemData* EquipItemData = UDBItemSingleton::Get().AddEquipItem(ItemID);
	UDBInventoryBlockWidget* Item = CreateWidget<UDBInventoryBlockWidget>(this, UDBInventoryBlockWidget::StaticClass());

	if (Item && EquipItemData)
	{	
		Item->SetEquipItemData(EquipItemData);
		switch (EquipItemData->GetEquipItemStat().ItemType)
		{
			case 0:
				WeaponItemBlocks->AddItem(Item);
				break;
			case 1:
				HeadItemBlocks->AddItem(Item);
				break;
			case 2:
				BodyItemBlocks->AddItem(Item);
				break;
			case 3:
				ShoesItemBlocks->AddItem(Item);
				break;
		}
	}
}

void UDBInventoryWidget::AddCountableItem(FName ItemID, int32 ItemCount)
{
	UDBCountableItemData* CountableItemData = UDBItemSingleton::Get().AddCountableItem(ItemID, ItemCount);

	UE_LOG(LogTemp, Log, TEXT("CountableItemData : %d"), CountableItemData->GetUniqueID());
	for (UObject* obj : CountableItemBlocks->GetListItems())
	{
		UE_LOG(LogTemp, Log, TEXT("CountableItemBlocks : %d"), obj->GetUniqueID());
	}

	//아이템이 인벤토리 블록으로 생성되어 있을 경우
	if (CountableItemBlocks->GetListItems().Contains(CountableItemData))
	{
		CountableItemData->SetItemCount();
		UE_LOG(LogTemp, Log, TEXT("Contain Item!"));
	}
	else
	{
		//UDBInventoryBlockWidget* Item = CreateWidget<UDBInventoryBlockWidget>(this, UDBInventoryBlockWidget::StaticClass());
		//Item->SetCountableItemData(CountableItemData);
		CountableItemBlocks->AddItem(CountableItemData);
	}
}

void UDBInventoryWidget::WeaponItemButtonCallback()
{
	SetAllInventoryCollapsed();
	WeaponItemBlocks->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryWidget::HeadItemButtonCallback()
{
	SetAllInventoryCollapsed();
	HeadItemBlocks->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryWidget::BodyItemButtonCallback()
{
	SetAllInventoryCollapsed();
	BodyItemBlocks->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryWidget::ShoesItemButtonCallback()
{
	SetAllInventoryCollapsed();
	ShoesItemBlocks->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryWidget::CountableItemButtonCallback()
{
	SetAllInventoryCollapsed();
	CountableItemBlocks->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryWidget::SetAllInventoryCollapsed()
{
	WeaponItemBlocks->SetVisibility(ESlateVisibility::Collapsed);
	HeadItemBlocks->SetVisibility(ESlateVisibility::Collapsed);
	BodyItemBlocks->SetVisibility(ESlateVisibility::Collapsed);
	ShoesItemBlocks->SetVisibility(ESlateVisibility::Collapsed);
	CountableItemBlocks->SetVisibility(ESlateVisibility::Collapsed);
}
