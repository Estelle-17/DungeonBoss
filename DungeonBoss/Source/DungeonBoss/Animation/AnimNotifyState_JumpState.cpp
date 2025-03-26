// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_JumpState.h"
#include "Interface/DBEnemyMotionWarpingInterface.h"

void UAnimNotifyState_JumpState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBEnemyMotionWarpingInterface* EnemyPawn = Cast<IDBEnemyMotionWarpingInterface>(MeshComp->GetOwner());
		if (EnemyPawn)
		{
			EnemyPawn->EnemyJumpStateBegin();
		}
	}
}

void UAnimNotifyState_JumpState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBEnemyMotionWarpingInterface* EnemyPawn = Cast<IDBEnemyMotionWarpingInterface>(MeshComp->GetOwner());
		if (EnemyPawn)
		{
			EnemyPawn->EnemyJumpStateEnd();
		}
	}
}
