// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_EnableGuardTime.h"
#include "Interface/DBAnimationNotifyInterface.h"

void UAnimNotifyState_EnableGuardTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->EnableGuardTime();
		}
	}
}

void UAnimNotifyState_EnableGuardTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->DisableGuardTime();
		}
	}
}
