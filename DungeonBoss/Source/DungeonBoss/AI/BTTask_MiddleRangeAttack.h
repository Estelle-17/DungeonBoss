// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MiddleRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UBTTask_MiddleRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MiddleRangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
