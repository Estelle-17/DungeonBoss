// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DBItemData.h"
#include "GameData/DBItemStat.h"
#include "GameData/DBCharacterStat.h"
#include "DBEquipItemData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBEquipItemData : public UDBItemData
{
	GENERATED_BODY()
	
public:
	void SetEquipStat(FName ItemID);

	UPROPERTY(VisibleAnywhere, Category = EquipMesh)
	TObjectPtr<class UStaticMeshComponent> EquipMesh;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBItemStat ItemStat;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat EquipCharacterStat;

protected:
	void SetEquipCharacterStat();
};
