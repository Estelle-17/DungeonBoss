// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIEnemyAttackFinished)
DECLARE_DELEGATE(FAITurnToTargetFinished)
/**
 * 
 */
class DUNGEONBOSS_API IDBEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAITurnToTargetDelegate(const FAIEnemyAttackFinished& InOnTurnToTargetFinished) = 0;
	virtual void AttackByAI(FString NewAttack) = 0;
	virtual void TurnToTargetByAI() = 0;

	FAIEnemyAttackFinished OnAttackFinished;
	FAITurnToTargetFinished OnTurnToTargetFinished;
};
