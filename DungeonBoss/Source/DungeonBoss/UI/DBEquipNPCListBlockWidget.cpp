// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBEquipNPCListBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"

UDBEquipNPCListBlockWidget::UDBEquipNPCListBlockWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBEquipNPCListBlockWidget::NativeConstruct()
{
	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameText")));
	ensure(ItemNameText);

	MenuPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MenuPanel")));
	ensure(MenuPanel);
}

void UDBEquipNPCListBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
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

void UDBEquipNPCListBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{

}
