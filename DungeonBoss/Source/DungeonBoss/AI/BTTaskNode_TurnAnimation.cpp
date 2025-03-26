// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TurnAnimation.h"
#include "DBAIController.h"
#include "Interface/DBEnemyAIInterface.h"

UBTTaskNode_TurnAnimation::UBTTaskNode_TurnAnimation()
{
}

EBTNodeResult::Type UBTTaskNode_TurnAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//제어하고 있는 폰의 정보를 가져옴
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IDBEnemyAIInterface* AIPawn = Cast<IDBEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FAITurnToTargetFinished OnTurnToTargetFinished;
	OnTurnToTargetFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAITurnToTargetDelegate(OnTurnToTargetFinished);
	AIPawn->TurnToTargetByAI();
	return EBTNodeResult::InProgress;
}
