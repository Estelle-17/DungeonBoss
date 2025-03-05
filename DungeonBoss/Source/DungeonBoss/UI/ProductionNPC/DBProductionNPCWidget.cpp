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
	//�� �κ��丮�� ĭ�� TArray�� ���� �� �ʱ�ȭ
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
		//MenuID�� ���� MenuObject�� ã�� �� SetOnGetItemChildren�� ����Ͽ� TreeView�� ���� ���� ����
		if (CheckContainMenu(NewMenuTable->MenuID) != -1)
		{
			UObject* RootObject = ItemTreeView->GetListItems()[CheckContainMenu(NewMenuTable->MenuID)];
			UDBMakeItemsData* RootItemData = Cast<UDBMakeItemsData>(RootObject);

			//Delegate����
			ItemData->OnMaterialForItemBlockSetting.AddUObject(this, &UDBProductionNPCWidget::SettingMaterialForItemBlock);
			ItemData->OnDisableAllMenuUI.AddUObject(this, &UDBProductionNPCWidget::DisableAllMenuUI);

			//UE_LOG(LogTemp, Log, TEXT("Check %s : %s\nAddMenuChild : %s"), *RootItemData->GetMenuID().ToString(), *ItemData->GetMenuID().ToString(), *ItemData->GetMenuName().ToString())

			RootItemData->ChildrenItems.Add(ItemData);
			ItemTreeView->SetOnGetItemChildren<UDBMakeItemsData>(RootItemData, &UDBMakeItemsData::OnGetItemChildren);
		}
	}
	else
	{
		//�ڽ� ��ü�� �ƴ� ��� �ٷ� TreeView�� �־���
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
	//�ε��� �Ϸ�ǰ� ��ᰡ ���� �����ϴ��� Ȯ��
	if (!InventoryWidget)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Create Item ID : %s"), *CurrentEquipItemIDOfSelectMenu.ToString()));

	for (int index = 0; index < 6; index++)
	{
		//������ ������ Ž��
		if (MaterialForItemBlocks[index]->GetVisibility() != ESlateVisibility::Collapsed)
		{
			//�ʿ��� ��Ḧ ����� ���������� �ʴٸ� return
			if (!MaterialForItemBlocks[index]->CheckCanCreateItem())
			{
				//������ �� �� ���ٴ� �޼��� ���
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Failed Create Item")));
				return;
			}
		}
	}

	//�������� �κ��丮�� ����
	InventoryWidget->AddEquipItem(CurrentEquipItemIDOfSelectMenu);
	//�� �������� �Ȼ��� ��	��

	//�����ϸ鼭 ���� ������ �κ��丮���� ����
	for (int index = 0; index < 6; index++)
	{
		//������ ������ Ž��
		if (MaterialForItemBlocks[index]->GetVisibility() != ESlateVisibility::Collapsed)
		{
			InventoryWidget->RemoveCountableItem(MaterialForItemBlocks[index]->GetItemID(), MaterialForItemBlocks[index]->NeedItemCount);
		}
	}

	DisableAllMenuUI();
}