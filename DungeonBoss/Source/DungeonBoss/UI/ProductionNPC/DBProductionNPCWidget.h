// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/DBMakeItemMenuTable.h"
#include "DBProductionNPCWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBProductionNPCWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDBProductionNPCWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//Variables
public:
	class ADBNPCBaseActor* NPCBaseActor;

//Widgets
//Variables
protected:
	UPROPERTY()
	class UTreeView* ItemTreeView;

	UPROPERTY()
	class UButton* MakeItemButton;

	TArray<class UDBNeedMaterialForItemWidget*> MaterialForItemBlocks;

//Functions
public:
	void SettingMaterialForItemBlock(FName NewEquipItemID, FName NewItemID, int NeedItemCount, int ItemNumber);

	UFUNCTION()
	void MakeItem();

protected:
	void AddItemMenu(FDBMakeItemMenuTable* NewMenuTable);
	int CheckContainMenu(FName NewMenuID);

//ItemsData
//Variables
protected:
	FName CurrentEquipItemIDOfSelectMenu;

//Inventory
//Variables
public:
	class UDBInventoryWidget* InventoryWidget;
//DataTable
//Functions
public:
	void SetAllMenuUI();
	void DisableAllMenuUI();

//Input Section
protected:
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
