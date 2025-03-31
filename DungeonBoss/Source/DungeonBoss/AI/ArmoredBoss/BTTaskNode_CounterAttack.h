// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_CounterAttack.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UBTTaskNode_CounterAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_CounterAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
