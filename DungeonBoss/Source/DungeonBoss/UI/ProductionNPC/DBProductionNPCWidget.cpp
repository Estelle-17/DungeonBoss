// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBProductionNPCWidget.h"
#include "Components/TreeView.h"
#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"
#include "GameData/DBItemSingleton.h"

UDBProductionNPCWidget::UDBProductionNPCWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBProductionNPCWidget::NativeConstruct()
{
	//�� �κ��丮�� ĭ�� TArray�� ���� �� �ʱ�ȭ
	for (int index = 0; index < 6; index++)
	{
		FName InventoryBlockName = *FString::Printf(TEXT("NeedMaterialForItemBlock_%d"), index);

	}

	//Widget Setting
	ItemTreeView = Cast<UTreeView>(GetWidgetFromName(TEXT("ItemTreeView")));
	ensure(ItemTreeView);
	ItemTreeView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UDBProductionNPCWidget::AddItemMenu(FDBMakeItemMenuTable* NewMenuTable)
{
	UDBMakeItemsData* ItemData = NewObject<UDBMakeItemsData>(this, UDBMakeItemsData::StaticClass());
	ItemData->SetItemData(NewMenuTable);

	if (ItemData->bIsChild)
	{
		//MenuID�� ���� MenuObject�� ã�� �� SetOnGetItemChildren�� ����Ͽ� TreeView�� ���� ���� ����
		if (CheckContainMenu(NewMenuTable->MenuID) != -1)
		{
			UObject* RootObject = ItemTreeView->GetListItems()[CheckContainMenu(NewMenuTable->MenuID)];
			UDBMakeItemsData* RootItemData = Cast<UDBMakeItemsData>(RootObject);

			UE_LOG(LogTemp, Log, TEXT("Check %s : %s\nAddMenuChild : %s"), *RootItemData->GetMenuID().ToString(), *ItemData->GetMenuID().ToString(), *ItemData->GetMenuName().ToString())

				RootItemData->ChildrenItems.Add(ItemData);
			ItemTreeView->SetOnGetItemChildren<UDBMakeItemsData>(RootItemData, &UDBMakeItemsData::OnGetItemChildren);
		}
	}
	else
	{
		ItemTreeView->AddItem(ItemData);
		UE_LOG(LogTemp, Log, TEXT("AddMenu : %s"), *ItemData->GetMenuName().ToString())
	}
}

int UDBProductionNPCWidget::CheckContainMenu(FName NewMenuID)
{
	TArray<UObject*> ItemDataArray = ItemTreeView->GetListItems();

	for (int index = 0; index < ItemDataArray.Num(); index++)
	{
		if (Cast<UDBMakeItemsData>(ItemDataArray[index])->GetMenuID().IsEqual(NewMenuID))
		{
			return index;
		}
	}
	return -1;
}

void UDBProductionNPCWidget::SetAllMenuUI()
{
	MakeItemMenuItems = UDBItemSingleton::Get().GetLoadMakeItemMenuTableData();

	for (int index = 0; index < MakeItemMenuItems.Num(); index++)
	{
		AddItemMenu(MakeItemMenuItems[index]);
	}
}