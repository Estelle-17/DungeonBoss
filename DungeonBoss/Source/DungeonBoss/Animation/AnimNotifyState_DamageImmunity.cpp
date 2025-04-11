// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_DamageImmunity.h"
#include "Interface/DBAnimationNotifyInterface.h"

void UAnimNotifyState_DamageImmunity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->DamageImmunityEnable();
		}
	}
}

void UAnimNotifyState_DamageImmunity::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->DamageImmunityDisable();
		}
	}
}
