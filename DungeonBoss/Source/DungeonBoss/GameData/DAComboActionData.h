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
	//Montage�� ������ ���λ�
	UPROPERTY(EditAnywhere, Category = Name)
	FString SectionPrefix;

	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;
	
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint32 MaxComboCount;
	
	//���� �޺��� �Ѿ�� �� �����ؾ� �ϴ� ������
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> RequireComboFrame;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> ComboFrame;
	
};
