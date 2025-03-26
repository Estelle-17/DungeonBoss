// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAArmoredBoss_CooltimeData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDAArmoredBoss_CooltimeData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDAArmoredBoss_CooltimeData();
public:
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> SkillCooltimes;
};
