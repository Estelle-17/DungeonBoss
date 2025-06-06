// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_TurnAnimation.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UBTTaskNode_TurnAnimation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_TurnAnimation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
