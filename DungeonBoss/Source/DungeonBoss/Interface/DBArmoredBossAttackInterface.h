// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBArmoredBossAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBArmoredBossAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONBOSS_API IDBArmoredBossAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SwordAttackCheck() = 0;
	virtual void ShieldAttackCheck() = 0;
	virtual void AroundAttackCheck() = 0;
	virtual void ForwardAttackCheck() = 0;
	virtual void ResetHitPlayers() = 0;
};
