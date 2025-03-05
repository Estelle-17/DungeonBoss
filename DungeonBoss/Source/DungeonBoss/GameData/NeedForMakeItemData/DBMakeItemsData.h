// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/DBMakeItemMenuTable.h"
#include "DBMakeItemsData.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FOnMaterialForItemBlockSetting, FName /*NewEquipItemID*/, FName /*NewItemID*/, int /*NeedItemCount*/, int /*ItemNumber*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckMakeItem, FName /*NewItemID*/);
DECLARE_MULTICAST_DELEGATE(FOnDisableAllMenuUI);
/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBMakeItemsData : public UObject
{
	GENERATED_BODY()
	
protected:
	UDBMakeItemsData();
	
public:
	void OnGetItemChildren(UObject* pItem, TArray<UObject*> &aChildren);

	FOnMaterialForItemBlockSetting OnMaterialForItemBlockSetting;
	FOnCheckMakeItem OnCheckMakeItem;
	FOnDisableAllMenuUI OnDisableAllMenuUI;

//ItemData
//Variables
public:
	UPROPERTY()
	bool bIsChild;

protected:
	//아이템 세트의 접두사
	UPROPERTY()
	FName MenuName;

	UPROPERTY()
	FName MenuID;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int SpacingSize;

//Functions
public:
	FORCEINLINE FName GetMenuName() { return MenuName; }
	FORCEINLINE FName GetMenuID() { return MenuID; }
	FORCEINLINE FName GetItemID() { return ItemID; }
	FORCEINLINE int GetSpacingSize() { return SpacingSize; }

	void SetItemData(FDBMakeItemMenuTable* NewMenuTable);
	
//Items
//Variables
public:
	UPROPERTY()
	TArray<UObject*> ChildrenItems;

};
