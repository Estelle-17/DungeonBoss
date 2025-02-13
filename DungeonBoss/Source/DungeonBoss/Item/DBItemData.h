// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DBItemData.generated.h"

UENUM(BlueprintType)
enum class EEquipItemType : uint8
{
	Weapon = 0,
	Head,
	Body,
	Shoes
};

UENUM(BlueprintType)
enum class ECountableItemType : uint8
{
	Material = 0,
};

UENUM(BlueprintType)
enum class EItemSlotType : uint8
{
	None = 0,
	Item
};

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EEquipItemType EquipItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	ECountableItemType CountableItemType;
};
