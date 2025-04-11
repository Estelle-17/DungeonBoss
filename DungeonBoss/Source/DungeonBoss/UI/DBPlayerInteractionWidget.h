// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBPlayerInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBPlayerInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDBPlayerInteractionWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateDescription(FString NewDescription);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> Description;

	UPROPERTY()
	TObjectPtr<class UTextBlock> InteractionKey;
};
