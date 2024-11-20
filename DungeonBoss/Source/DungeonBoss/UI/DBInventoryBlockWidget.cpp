// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DBInventoryBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UDBInventoryBlockWidget::UDBInventoryBlockWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UDBInventoryBlockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquipItemData = NewObject<UDBEquipItemData>(this, UDBEquipItemData::StaticClass());

	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	ensure(ItemCount);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);
}

void UDBInventoryBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	//ItemCount->SetText(FText::FromString(EquipItemData->ItemStat));
}

void UDBInventoryBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

}

void UDBInventoryBlockWidget::SetEquipItem(FName ItemID)
{
	EquipItemData->SetEquipStat(ItemID);
}
