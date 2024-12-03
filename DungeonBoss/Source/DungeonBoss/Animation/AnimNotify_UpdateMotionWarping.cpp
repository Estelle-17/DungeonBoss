// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_UpdateMotionWarping.h"
#include "Interface/DBMotionWarpingInterface.h"

void UAnimNotify_UpdateMotionWarping::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBMotionWarpingInterface* AttackPawn = Cast<IDBMotionWarpingInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->UpdateMotionWarpingTargetVector();
		}
	}
}
