// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ArmoredBoss/BTTaskNode_CloseShieldAttack.h"
#include "AI/DBAIController.h"
#include "Interface/DBEnemyAIInterface.h"

UBTTaskNode_CloseShieldAttack::UBTTaskNode_CloseShieldAttack()
{
}

EBTNodeResult::Type UBTTaskNode_CloseShieldAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//�����ϰ� �ִ� ���� ������ ������
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
	AIPawn->AttackByAI("ShieldAttack");
	return EBTNodeResult::InProgress;
}
