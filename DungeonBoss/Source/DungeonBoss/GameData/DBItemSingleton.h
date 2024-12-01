// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBItemStat.h"
#include "DBCharacterStat.h"
#include "Item/Equip/DBEquipItemData.h"
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
	FORCEINLINE FDBItemStat* GetItemStat(FName ItemID) { return ItemDataTable->FindRow<FDBItemStat>(ItemID, TEXT("")); }
	FORCEINLINE const TArray<UDBEquipItemData*> GetEquipItems(FName ItemID) { return EquipItems; }

	UDBEquipItemData* AddEquipItem(FName ItemID);
	void RemoveEquipItem(UDBEquipItemData* EquipItemData);
	void AddCountableItem(FName ItemID);

private:
	class UDataTable* ItemDataTable;

	TArray<UDBEquipItemData*> EquipItems;
};
