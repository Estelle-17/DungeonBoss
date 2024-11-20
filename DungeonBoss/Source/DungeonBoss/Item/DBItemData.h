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

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EEquipItemType Type;
};
