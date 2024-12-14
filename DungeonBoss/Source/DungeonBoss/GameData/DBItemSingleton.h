// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBItemStat.h"
#include "DBCharacterStat.h"
#include "Item/Equip/DBEquipItemData.h"
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
	FORCEINLINE FDBItemStat* GetEquipItemStat(FName ItemID) { return EquipItemDataTable->FindRow<FDBItemStat>(ItemID, TEXT("")); }
	FORCEINLINE const TArray<UDBEquipItemData*> GetEquipItems(FName ItemID) { return EquipItems; }

	FORCEINLINE FDBMaterialItemStat* GetCountableItemStat(FName ItemID) { return CountableItemDataTable->FindRow<FDBMaterialItemStat>(ItemID, TEXT("")); }
	//FORCEINLINE const UDBCountableItemData* GetCountableItem(FName ItemID) { return CountableItems.Find(ItemID)->CountableItem; }

	UDBEquipItemData* AddEquipItem(FName ItemID);
	void RemoveEquipItem(UDBEquipItemData* EquipItemData);
	UDBCountableItemData* AddCountableItem(FName ItemID, int32 ItemCount);

private:
	class UDataTable* EquipItemDataTable;

	UPROPERTY(EditAnywhere)
	TArray<UDBEquipItemData*> EquipItems;

	class UDataTable* CountableItemDataTable;

	UPROPERTY(EditAnywhere)
	TMap<FName, UDBCountableItemData*> CountableItems;
};
