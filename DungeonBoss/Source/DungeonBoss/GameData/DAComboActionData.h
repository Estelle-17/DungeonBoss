// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDAComboActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UDAComboActionData();
	
public:
	//Montage내 섹션의 접두사
	UPROPERTY(EditAnywhere, Category = Name)
	FString SectionPrefix;

	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;
	
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint32 MaxComboCount;
	
	//다음 콤보로 넘어가기 전 실행해야 하는 프레임
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> RequireComboFrame;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> ComboFrame;
	
};
