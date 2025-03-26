// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckCanAttack.h"
#include "DBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DBAI.h"
#include "DrawDebugHelpers.h"
#include "Interface/DBEnemyAIInterface.h"
#include "Engine/OverlapResult.h"
#include "Enemy/DBEnemyBase.h"

UBTService_CheckCanAttack::UBTService_CheckCanAttack()
{
}

void UBTService_CheckCanAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//�����ϰ� �ִ� ���� ������ ������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	IDBEnemyAIInterface* AIPawn = Cast<IDBEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return;
	}

	//Target-Enemy�� �Ÿ� ���
	float DistanceToTaget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("DistanceToTaget : %f, AttackRange : %f"), DistanceToTaget, AttackRangeWithRadius));
	bool bResult = (DistanceToTaget <= AttackRangeWithRadius);
	//������ �Ÿ� ���� Ÿ���� ���� ���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_CLOSERANGE_ATTACK, bResult);
	//������ �Ÿ����� �ְ�, ������ �Ÿ��� �� �� ���� Ÿ���� ���� ���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_MIDDLERANGE_ATTACK,(AttackRangeWithRadius <= DistanceToTaget) && (DistanceToTaget <= AttackRangeWithRadius * 2));
	//������ �Ÿ��� �� �躸�� �ְ�, ������ �Ÿ��� �� �� ���� Ÿ���� ���� ���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_LONGRANGE_ATTACK, (AttackRangeWithRadius*2 <= DistanceToTaget) && (DistanceToTaget <= AttackRangeWithRadius*3));
}
