// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBCharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONBOSS_API IDBCharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(class UDBHUDWidget* InHUDWidget) = 0;
	virtual void SetupInventoryWidget(class UDBInventoryWidget* InInventoryWidget) = 0;
};
