// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTDecorator_AttackInLongRange.h"
#include "DBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DBAI.h"
#include "Interface/DBEnemyAIInterface.h"

UBTDecorator_AttackInLongRange::UBTDecorator_AttackInLongRange()
{
	NodeName = TEXT("CanLongAttack");
}

bool UBTDecorator_AttackInLongRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//�����ϰ� �ִ� ���� ������ ������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IDBEnemyAIInterface* AIPawn = Cast<IDBEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return false;
	}

	//Target-Enemy�� �Ÿ� ���
	float DistanceToTaget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange() * 3;
	//GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("DistanceToTaget : %f, AttackRange : %f"), DistanceToTaget, AttackRangeWithRadius));
	bResult = ((AttackRangeWithRadius * 0.5) <= DistanceToTaget) && (DistanceToTaget <= AttackRangeWithRadius);
	return bResult;
}
