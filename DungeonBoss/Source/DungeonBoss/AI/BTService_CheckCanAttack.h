// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckCanAttack.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UBTService_CheckCanAttack : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckCanAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
