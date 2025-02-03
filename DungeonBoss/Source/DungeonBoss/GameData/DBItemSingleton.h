// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBItemStat.h"
#include "DBCharacterStat.h"
#include "Item/DBItemObject.h"
#include "Item/DBCountableItemData.h"
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
	void AddEquipItem(FName ItemID, UDBItemObject* ItemObject);
	void RemoveEquipItem(UDBItemObject* ItemObject);
	void AddCountableItem(FName ItemID, UDBItemObject* ItemObject);

//EquipItemData
	FORCEINLINE FDBItemStat* GetLoadEquipItemData(FName ItemID) { return EquipItemDataTable->FindRow<FDBItemStat>(ItemID, TEXT("")); }
	FORCEINLINE const TArray<UDBItemObject*> GetEquipItemObjects() { return EquipItems; }
//CountableItemData
	FORCEINLINE FDBMaterialItemStat* GetLoadCountableItemData(FName ItemID) { return CountableItemDataTable->FindRow<FDBMaterialItemStat>(ItemID, TEXT("")); }
	FORCEINLINE UDBItemObject* GetCountableItemObject(FName ItemID) { return CountableItems[ItemID]; }
	FORCEINLINE bool IsContainCountableItem(FName ItemID) { return CountableItems.Contains(ItemID); }


private:
	class UDataTable* EquipItemDataTable;

	UPROPERTY(EditAnywhere)
	TArray<UDBItemObject*> EquipItems;

	class UDataTable* CountableItemDataTable;

	UPROPERTY(EditAnywhere)
	TMap<FName, UDBItemObject*> CountableItems;
};
