// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBCharacterInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBCharacterInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBCharacterInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
};
