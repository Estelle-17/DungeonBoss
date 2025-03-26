// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/ArmoredBoss/AnimNotifyState_ShieldHitCheck.h"
#include "Interface/DBArmoredBossAttackInterface.h"

void UAnimNotifyState_ShieldHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ElapsedTime += FrameDeltaTime;

	if (MeshComp && ElapsedTime >= TickInterval)
	{
		IDBArmoredBossAttackInterface* EnemyPawn = Cast<IDBArmoredBossAttackInterface>(MeshComp->GetOwner());

		if (EnemyPawn)
		{
			EnemyPawn->ShieldAttackCheck();
		}

		ElapsedTime = 0.0f;
	}
}

void UAnimNotifyState_ShieldHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBArmoredBossAttackInterface* EnemyPawn = Cast<IDBArmoredBossAttackInterface>(MeshComp->GetOwner());

		if (EnemyPawn)
		{
			EnemyPawn->ResetHitPlayers();
		}
	}
}
