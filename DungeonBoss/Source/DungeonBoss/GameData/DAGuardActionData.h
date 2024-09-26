// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAGuardActionData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDAGuardActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UDAGuardActionData();

public:
	//Montage내 섹션의 접두사
	UPROPERTY(EditAnywhere, Category = Name)
	FString SectionPrefix;

	UPROPERTY(EditAnywhere, Category = GuardData)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = GuardData)
	float RequireGuardFrame;

	UPROPERTY(EditAnywhere, Category = GuardData)
	TArray<float> GuardFrame;
};
