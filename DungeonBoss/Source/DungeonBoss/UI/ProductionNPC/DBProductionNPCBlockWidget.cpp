// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBProductionNPCBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"

UDBProductionNPCBlockWidget::UDBProductionNPCBlockWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBProductionNPCBlockWidget::NativeConstruct()
{
	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameText")));
	ensure(ItemNameText);

	MenuPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MenuPanel")));
	ensure(MenuPanel);
}

void UDBProductionNPCBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UDBMakeItemsData* ItemData = Cast<UDBMakeItemsData>(ListItemObject);

	MenuPanel->SetRenderTranslation(FVector2D(ItemData->GetSpacingSize(), 0.0f));

	if (ItemData)
	{
		if (ItemData->bIsChild)
		{
			ItemNameText->SetText(FText::FromName(ItemData->GetMenuName()));
		}
		else
		{
			ItemNameText->SetText(FText::FromString(FString::Printf(TEXT("+ %s"), *ItemData->GetMenuName().ToString())));
		}
	}
}

void UDBProductionNPCBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{

}