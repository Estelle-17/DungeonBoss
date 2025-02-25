// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/DBMakeItemMenuTable.h"
#include "DBMakeItemsData.generated.h"

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
	int SpacingSize;

//Functions
public:
	FORCEINLINE FName GetMenuName() { return MenuName; }
	FORCEINLINE FName GetMenuID() { return MenuID; }
	FORCEINLINE int GetSpacingSize() { return SpacingSize; }

	void SetItemData(FDBMakeItemMenuTable* NewMenuTable);
	
//Items
//Variables
public:
	UPROPERTY()
	TArray<UObject*> ChildrenItems;

};
