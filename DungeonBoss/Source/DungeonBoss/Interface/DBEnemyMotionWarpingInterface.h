// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBEnemyMotionWarpingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBEnemyMotionWarpingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONBOSS_API IDBEnemyMotionWarpingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CheckTagetLocationBegin() = 0;
	virtual void CheckTagetLocationEnd() = 0;
};
