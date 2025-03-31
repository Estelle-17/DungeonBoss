// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/ArmoredBoss/AnimNotifyState_CheckCounter.h"
#include "Interface/DBArmoredBossAttackInterface.h"

void UAnimNotifyState_CheckCounter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBArmoredBossAttackInterface* EnemyPawn = Cast<IDBArmoredBossAttackInterface>(MeshComp->GetOwner());

		if (EnemyPawn)
		{
			EnemyPawn->CounterStateBegin();
		}
	}
}

void UAnimNotifyState_CheckCounter::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBArmoredBossAttackInterface* EnemyPawn = Cast<IDBArmoredBossAttackInterface>(MeshComp->GetOwner());

		if (EnemyPawn)
		{
			EnemyPawn->CounterStateEnd();
		}
	}
}
