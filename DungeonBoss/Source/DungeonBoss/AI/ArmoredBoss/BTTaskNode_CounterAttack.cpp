// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArmoredBoss/BTTaskNode_CounterAttack.h"
#include "AI/DBAIController.h"
#include "Interface/DBEnemyAIInterface.h"

UBTTaskNode_CounterAttack::UBTTaskNode_CounterAttack()
{
}

EBTNodeResult::Type UBTTaskNode_CounterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FAIEnemyAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI("CounterPose");
	return EBTNodeResult::InProgress;
}
