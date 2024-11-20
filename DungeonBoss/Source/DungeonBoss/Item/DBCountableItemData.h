// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DBCountableItemData.generated.h"

UENUM(BlueprintType)
enum class ECountableItemType : uint8
{
	Material = 0
};

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBCountableItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	ECountableItemType Type;
};
