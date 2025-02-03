// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DBItemData.h"
#include "GameData/DBMaterialItemStat.h"
#include "GameData/DBCharacterStat.h"

#include "DBCountableItemData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBCountableItemData : public UDBItemData
{
	GENERATED_BODY()
	
public:
	void SetCountableItemStat(FName ItemID, int32 Count);

	FORCEINLINE void SetItemStat(const FDBMaterialItemStat& InItemStat) { ItemStat = InItemStat; }

	FORCEINLINE const FDBMaterialItemStat GetItemStat() { return ItemStat; }
	FORCEINLINE UTexture2D* GetItemTexture() { return ItemTexture; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Texture)
	UTexture2D* ItemTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBMaterialItemStat ItemStat;
};
