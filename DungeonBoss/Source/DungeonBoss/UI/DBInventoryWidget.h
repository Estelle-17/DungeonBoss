// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void AddEquipItem(FName ItemID);

protected:
	UPROPERTY()
	TObjectPtr<class UTileView> ItemBlocks;
};
