// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToForward.h"
#include "DBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DBAI.h"
#include "Interface/DBEnemyAIInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_TurnToForward::UBTTask_TurnToForward()
{
	NodeName = TEXT("TurnToMoveForward");
}

EBTNodeResult::Type UBTTask_TurnToForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//�����ϰ� �ִ� ���� ������ ������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IDBEnemyAIInterface* AIPawn = Cast<IDBEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* Owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Owner == nullptr)
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

	float TurnSpeed = AIPawn->GetAITurnSpeed();
	FVector LookVector = Owner->GetCharacterMovement()->Velocity;
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
