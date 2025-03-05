// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBProductionNPCBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"
#include "GameData/DBMaterialForMakeItemTable.h"
#include "GameData/DBItemSingleton.h"

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
	MakeItemData = Cast<UDBMakeItemsData>(ListItemObject);

	MenuPanel->SetRenderTranslation(FVector2D(MakeItemData->GetSpacingSize(), 0.0f));

	if (MakeItemData)
	{
		if (MakeItemData->bIsChild)
		{
			ItemNameText->SetText(FText::FromName(MakeItemData->GetMenuName()));
		}
		else
		{
			ItemNameText->SetText(FText::FromString(FString::Printf(TEXT("+ %s"), *MakeItemData->GetMenuName().ToString())));
		}
	}
}

void UDBProductionNPCBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (!MakeItemData->bIsChild)
	{
		return;
	}

	if (bIsSelected)
	{
		SetItemUISetting();
	}
}

void UDBProductionNPCBlockWidget::SetItemUISetting()
{
	if (MakeItemData)
	{
		MakeItemData->OnDisableAllMenuUI.Broadcast();

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Clicked %s : %s"), *MakeItemData->GetMenuName().ToString(), *MakeItemData->GetMenuID().ToString()));

		FDBMaterialForMakeItemTable* MakeItemTable = UDBItemSingleton::Get().GetLoadDBMaterialForMakeItemTableData(MakeItemData->GetItemID());

		//,로 String을 나누어 각 재료를 UI에 넣어줌
		TArray<FString> ItemArray;
		MakeItemTable->NeedMaterial.ParseIntoArray(ItemArray, TEXT(","));
		TArray<FString> ItemCountArray;
		MakeItemTable->NeedMaterialCount.ParseIntoArray(ItemCountArray, TEXT(","));

		//순서대로 UI생성
		for (int index = 0; index < ItemArray.Num(); index++)
		{
			MakeItemData->OnMaterialForItemBlockSetting.Broadcast(MakeItemData->GetItemID(), FName(*ItemArray[index]), FCString::Atoi(*ItemCountArray[index]), index);
		}
	}
}
