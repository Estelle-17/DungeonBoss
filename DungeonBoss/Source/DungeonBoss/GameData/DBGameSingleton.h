// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBItemStat.h"
#include "DBCharacterStat.h"
#include "DBGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDBGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UDBGameSingleton();
	static UDBGameSingleton& Get();

public:
	FORCEINLINE FDBItemStat* GetItemStat(FName ItemID) { return ItemDataTable->FindRow<FDBItemStat>(ItemID, TEXT("")); }

private:
	class UDataTable* ItemDataTable;
};
