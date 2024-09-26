// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_EnableComboTime.h"
#include "Interface/DBAnimationNotifyInterface.h"

void UAnimNotifyState_EnableComboTime::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (MeshComp)
	{
		IDBAnimationNotifyInterface* AttackPawn = Cast<IDBAnimationNotifyInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->CheckEnableComboTime();
		}
	}
}
