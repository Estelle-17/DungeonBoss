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

	//제어하고 있는 폰의 정보를 가져옴
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

	//Target-Enemy의 거리 계산
	float DistanceToTaget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("DistanceToTaget : %f, AttackRange : %f"), DistanceToTaget, AttackRangeWithRadius));
	bool bResult = (DistanceToTaget <= AttackRangeWithRadius);
	//정해진 거리 내에 타겟이 있을 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_CLOSERANGE_ATTACK, bResult);
	//정해진 거리보다 멀고, 정해진 거리의 두 배 내에 타겟이 있을 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_MIDDLERANGE_ATTACK,(AttackRangeWithRadius <= DistanceToTaget) && (DistanceToTaget <= AttackRangeWithRadius * 2));
	//정해진 거리의 두 배보다 멀고, 정해진 거리의 세 배 내에 타겟이 있을 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_CAN_LONGRANGE_ATTACK, (AttackRangeWithRadius*2 <= DistanceToTaget) && (DistanceToTaget <= AttackRangeWithRadius*3));
}
