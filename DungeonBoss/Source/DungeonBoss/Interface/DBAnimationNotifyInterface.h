// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBAnimationNotifyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBAnimationNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONBOSS_API IDBAnimationNotifyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CheckEnableComboTime() = 0;
	virtual void EnableGuardTime() = 0;
	virtual void DisableGuardTime() = 0;
	virtual void AnimationOutEnable() = 0;
	virtual void AnimationOutDisable() = 0;
	virtual void DamageImmunityEnable() = 0;
	virtual void DamageImmunityDisable() = 0;
};
