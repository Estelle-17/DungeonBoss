// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AttackHitCheck.h"
#include "Interface/DBAnimationAttackInterface.h"

void UAnimNotifyState_AttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBAnimationAttackInterface* AttackPawn = Cast<IDBAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->CheckHitEnable();
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBAnimationAttackInterface* AttackPawn = Cast<IDBAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->CheckHitDisable();
		}
	}
}
