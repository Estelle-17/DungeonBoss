// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAChargeAttackActionData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDAChargeAttackActionData : public UDataAsset
{
	GENERATED_BODY()
public:
	UDAChargeAttackActionData();
public:
	//Montage�� ������ ���λ�
	UPROPERTY(EditAnywhere, Category = Name)
	FString SectionPrefix;

	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	uint32 MaxChargeStack;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> RequireChargeFrame;
};
