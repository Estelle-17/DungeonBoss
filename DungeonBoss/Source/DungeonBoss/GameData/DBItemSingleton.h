// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBItemStat.h"
#include "DBCharacterStat.h"
#include "Item/DBItemObject.h"
#include "Item/DBCountableItemData.h"
#include "GameData/DBMakeItemMenuTable.h"
#include "GameData/DBMaterialForMakeItemTable.h"
#include "GameData/DBEnemyStat.h"
#include "DBItemSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDBItemSingleton, Error, All);

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemSingleton : public UObject
{
	GENERATED_BODY()

public:
	UDBItemSingleton();
	static UDBItemSingleton& Get();

public:
//EquipItemData
	FORCEINLINE FDBItemStat* GetLoadEquipItemData(FName ItemID) { return EquipItemDataTable->FindRow<FDBItemStat>(ItemID, TEXT("")); }
//CountableItemData
	FORCEINLINE FDBMaterialItemStat* GetLoadCountableItemData(FName ItemID) { return CountableItemDataTable->FindRow<FDBMaterialItemStat>(ItemID, TEXT("")); }
	FORCEINLINE int GetCountableItemCount(FName ItemID) { return CountableItems[ItemID]; }
	void SetCountableItemObject(FName ItemID, int ItemCount);
	FORCEINLINE bool IsContainCountableItem(FName ItemID) { return CountableItems.Contains(ItemID); }
//MakeItemMenuTable
	FORCEINLINE TArray<FDBMakeItemMenuTable*> GetLoadMakeItemMenuTableData() { return MakeItemMenuItems; }
//MaterialForMakeItemTable
	FORCEINLINE FDBMaterialForMakeItemTable* GetLoadDBMaterialForMakeItemTableData(FName ItemID) { return MaterialForMakeItemTable->FindRow<FDBMaterialForMakeItemTable>(ItemID, TEXT("")); }
//EnemyStatTable
	FORCEINLINE FDBEnemyStat* GetLoadDBEnemyStatTableData(FName ItemID) { return EnemyStatTable->FindRow<FDBEnemyStat>(ItemID, TEXT("")); }
private:
	class UDataTable* EquipItemDataTable;

	class UDataTable* CountableItemDataTable;

	UPROPERTY(EditAnywhere)
	TMap<FName, int32> CountableItems;

	class UDataTable* MakeItemMenuTable;
	TArray<FDBMakeItemMenuTable*> MakeItemMenuItems;

	class UDataTable* MaterialForMakeItemTable;

	class UDataTable* EnemyStatTable;
	TArray<FDBEnemyStat*> EnemyStatTableItems;
};
