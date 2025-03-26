// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/ArmoredBoss/AnimNotify_ForwardAttack.h"
#include "Interface/DBArmoredBossAttackInterface.h"

void UAnimNotify_ForwardAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IDBArmoredBossAttackInterface* EnemyPawn = Cast<IDBArmoredBossAttackInterface>(MeshComp->GetOwner());

		if (EnemyPawn)
		{
			EnemyPawn->ForwardAttackCheck();
		}
	}
}
