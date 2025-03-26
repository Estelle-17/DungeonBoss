// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArmoredBoss/BTTaskNode_CloseTurnAttack.h"
#include "AI/DBAIController.h"
#include "Interface/DBEnemyAIInterface.h"

UBTTaskNode_CloseTurnAttack::UBTTaskNode_CloseTurnAttack()
{
}

EBTNodeResult::Type UBTTaskNode_CloseTurnAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AIPawn->AttackByAI("TurnAttack");
	return EBTNodeResult::InProgress;
}
