// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DBEnemyMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBEnemyMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UDBEnemyMovementComponent();
};
