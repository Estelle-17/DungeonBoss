// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/DBMakeItemMenuTable.h"
#include "DBEquipNPCWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBEquipNPCWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBEquipNPCWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//Widgets
//Variables
protected:
	UPROPERTY()
	class UTreeView* ItemTreeView;

//Functions
protected:
	void AddItemMenu(FDBMakeItemMenuTable* NewMenuTable);
	int CheckContainMenu(FName NewMenuID);

//DataTable
//Variables
protected:
	class UDataTable* MakeItemMenuTable;

	TArray<FDBMakeItemMenuTable*> MakeItemMenuItems;

//Functions
public:
	void SetAllMenuUI();
};
