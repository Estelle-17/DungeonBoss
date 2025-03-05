// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBProductionNPCWidget.h"
#include "UI/ProductionNPC/DBNeedMaterialForItemWidget.h"
#include "Components/TreeView.h"
#include "Components/Button.h"
#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"
#include "UI/Inventory/DBInventoryWidget.h"
#include "GameData/DBItemSingleton.h"

UDBProductionNPCWidget::UDBProductionNPCWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBProductionNPCWidget::NativeConstruct()
{
	//각 인벤토리의 칸을 TArray에 저장 및 초기화
	for (int index = 0; index < 6; index++)
	{
		FName InventoryBlockName = *FString::Printf(TEXT("NeedMaterialForItemBlock_%d"), index);
		UDBNeedMaterialForItemWidget* ItemBlock = Cast<UDBNeedMaterialForItemWidget>(GetWidgetFromName(InventoryBlockName));
		ensure(ItemBlock);
		MaterialForItemBlocks.Add(ItemBlock);
		ItemBlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	//Widget Setting
	ItemTreeView = Cast<UTreeView>(GetWidgetFromName(TEXT("ItemTreeView")));
	ensure(ItemTreeView);
	ItemTreeView->SetScrollbarVisibility(ESlateVisibility::Hidden);

	MakeItemButton = Cast<UButton>(GetWidgetFromName(TEXT("MakeButton")));
	ensure(MakeItemButton);
	MakeItemButton->OnClicked.AddDynamic(this, &UDBProductionNPCWidget::MakeItem);
}

void UDBProductionNPCWidget::AddItemMenu(FDBMakeItemMenuTable* NewMenuTable)
{
	UDBMakeItemsData* ItemData = NewObject<UDBMakeItemsData>(this, UDBMakeItemsData::StaticClass());
	ItemData->SetItemData(NewMenuTable);

	if (ItemData->bIsChild)
	{
		//MenuID가 같은 MenuObject를 찾은 후 SetOnGetItemChildren을 사용하여 TreeView의 계층 구조 제작
		if (CheckContainMenu(NewMenuTable->MenuID) != -1)
		{
			UObject* RootObject = ItemTreeView->GetListItems()[CheckContainMenu(NewMenuTable->MenuID)];
			UDBMakeItemsData* RootItemData = Cast<UDBMakeItemsData>(RootObject);

			//Delegate설정
			ItemData->OnMaterialForItemBlockSetting.AddUObject(this, &UDBProductionNPCWidget::SettingMaterialForItemBlock);
			ItemData->OnDisableAllMenuUI.AddUObject(this, &UDBProductionNPCWidget::DisableAllMenuUI);

			//UE_LOG(LogTemp, Log, TEXT("Check %s : %s\nAddMenuChild : %s"), *RootItemData->GetMenuID().ToString(), *ItemData->GetMenuID().ToString(), *ItemData->GetMenuName().ToString())

			RootItemData->ChildrenItems.Add(ItemData);
			ItemTreeView->SetOnGetItemChildren<UDBMakeItemsData>(RootItemData, &UDBMakeItemsData::OnGetItemChildren);
		}
	}
	else
	{
		//자식 개체가 아닌 경우 바로 TreeView에 넣어줌
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
	TArray<FDBMakeItemMenuTable*> MakeItemMenuItems = UDBItemSingleton::Get().GetLoadMakeItemMenuTableData();

	for (int index = 0; index < MakeItemMenuItems.Num(); index++)
	{
		AddItemMenu(MakeItemMenuItems[index]);
	}
}

void UDBProductionNPCWidget::DisableAllMenuUI()
{
	for (int index = 0; index < 6; index++)
	{
		MaterialForItemBlocks[index]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDBProductionNPCWidget::SettingMaterialForItemBlock(FName NewEquipItemID, FName NewItemID, int NeedItemCount, int ItemNumber)
{
	MaterialForItemBlocks[ItemNumber]->SetVisibility(ESlateVisibility::Visible);

	MaterialForItemBlocks[ItemNumber]->SetBlockSetting(NewItemID, UDBItemSingleton::Get().GetCountableItemCount(NewItemID), NeedItemCount);
	CurrentEquipItemIDOfSelectMenu = NewEquipItemID;
}

void UDBProductionNPCWidget::MakeItem()
{
	//로딩이 완료되고 재료가 전부 존재하는지 확인
	if (!InventoryWidget)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Create Item ID : %s"), *CurrentEquipItemIDOfSelectMenu.ToString()));

	for (int index = 0; index < 6; index++)
	{
		//재료들의 정보를 탐색
		if (MaterialForItemBlocks[index]->GetVisibility() != ESlateVisibility::Collapsed)
		{
			//필요한 재료를 충분히 가지고있지 않다면 return
			if (!MaterialForItemBlocks[index]->CheckCanCreateItem())
			{
				//제작을 할 수 없다는 메세지 출력
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Failed Create Item")));
				return;
			}
		}
	}

	//아이템을 인벤토리에 생성
	InventoryWidget->AddEquipItem(CurrentEquipItemIDOfSelectMenu);
	//왜 아이템이 안생겨 ㅅ	ㅂ

	//생성하면서 사용된 재료들을 인벤토리에서 제거
	for (int index = 0; index < 6; index++)
	{
		//재료들의 정보를 탐색
		if (MaterialForItemBlocks[index]->GetVisibility() != ESlateVisibility::Collapsed)
		{
			InventoryWidget->RemoveCountableItem(MaterialForItemBlocks[index]->GetItemID(), MaterialForItemBlocks[index]->NeedItemCount);
		}
	}

	DisableAllMenuUI();
}