// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToForward.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UBTTask_TurnToForward : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnToForward();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
