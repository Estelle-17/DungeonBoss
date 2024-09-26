// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AnimOutTime.h"
#include "Interface/DBAnimationNotifyInterface.h"

void UAnimNotifyState_AnimOutTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AnimationOutEnable();
		}
	}
}

void UAnimNotifyState_AnimOutTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AnimationOutDisable();
		}
	}
}