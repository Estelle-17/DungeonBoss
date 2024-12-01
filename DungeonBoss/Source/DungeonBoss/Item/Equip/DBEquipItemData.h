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

	FORCEINLINE void SetItemStat(const FDBItemStat& InItemStat) { ItemStat = InItemStat; }
	FORCEINLINE void SetCharacterStat(const FDBCharacterStat& InCharacterStat) { EquipCharacterStat = InCharacterStat; }

	FORCEINLINE const FDBCharacterStat& GetEquipCharacterStat() const { return EquipCharacterStat; }
	FORCEINLINE const FDBItemStat& GetEquipItemStat() const { return ItemStat; }
	FORCEINLINE UTexture2D* GetItemTexture() { return ItemTexture; }

protected:
	UPROPERTY(VisibleAnywhere, Category = EquipMesh)
	TObjectPtr<class UStaticMeshComponent> EquipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Texture)
	UTexture2D* ItemTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBItemStat ItemStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat EquipCharacterStat;

	void SetEquipCharacterStat();

};
