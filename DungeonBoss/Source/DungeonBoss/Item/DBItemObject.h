// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/DBItemStat.h"
#include "GameData/DBCharacterStat.h"
#include "Item/Equip/DBEquipItemData.h"
#include "Item/DBCountableItemData.h"
#include "DBItemObject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSetItemCount);
/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UDBItemObject();

public:
//Get, Set¼³Á¤
//UDBEquipItemData
	FORCEINLINE UDBEquipItemData* GetEquipItemData() { return EquipItemData; }

//UDBCountableItemData
	FORCEINLINE UDBCountableItemData* GetCountableItemData() { return CountableItemData; }

	FORCEINLINE void SetItemCount(int32 Count) { CountableItemCount += Count; OnSetItemCount.Broadcast(); }
	FORCEINLINE void SetItemCount() { OnSetItemCount.Broadcast(); }
	FORCEINLINE const int32 GetItemCount() { return CountableItemCount; }

public:
	void MakeEquipItemData(FName ItemID);
	void MakeCountableItemData(FName ItemID, int32 ItemCount);

	uint8 bIsCountableItem = 0;

	FOnSetItemCount OnSetItemCount;
	
private:
	UDBEquipItemData* EquipItemData;

	UDBCountableItemData* CountableItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CountableItemCount = 0;
};
