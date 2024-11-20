// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DBInventoryWidget.h"
#include "Components/TileView.h"
#include "UI/DBInventoryBlockWidget.h"
#include "Interface/DBCharacterHUDInterface.h"
#include "DungeonBoss.h"

UDBInventoryWidget::UDBInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UDBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemBlocks = Cast<UTileView>(GetWidgetFromName(TEXT("ItemBlocks")));
	ensure(ItemBlocks);

	IDBCharacterHUDInterface* HUDPawn = Cast<IDBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupInventoryWidget(this);
	}
}

void UDBInventoryWidget::AddEquipItem(FName ItemID)
{
	UDBInventoryBlockWidget* Item = NewObject<UDBInventoryBlockWidget>(this, UDBInventoryBlockWidget::StaticClass());
	Item->SetEquipItem(ItemID);

	//ItemBlocks->AddItem(Item);
}
