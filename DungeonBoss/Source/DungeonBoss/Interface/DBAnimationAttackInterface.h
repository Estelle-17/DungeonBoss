// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONBOSS_API IDBAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CheckHitEnable() = 0;
	virtual void CheckHitDisable() = 0;
};
