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

	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	ensure(ItemCount);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);
}

void UDBInventoryBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (bIsCountableItem)
	{

	}
	else
	{
		SetEquipItemSetting(ListItemObject);
	}
}

void UDBInventoryBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	if (bIsSelected)
	{
		UE_LOG(LogTemp, Log, TEXT("%s is Select true"), *this->GetFName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s is Select false"), *this->GetFName().ToString());
	}
	
}

void UDBInventoryBlockWidget::SetEquipItemSetting(UObject* ListItemObject)
{
	EquipItemData = Cast<UDBInventoryBlockWidget>(ListItemObject)->GetEquipItemData();

	ItemCount->SetText(FText::FromString(TEXT("")));
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}
