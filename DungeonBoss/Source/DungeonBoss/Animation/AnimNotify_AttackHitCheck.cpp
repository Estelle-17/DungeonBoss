// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/DBAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//Animation Montage���� Notify�� �����Ͽ��� �� �����ų �⺻ ���� �Լ� ����
	if (MeshComp)
	{
		IDBAnimationAttackInterface* AttackPawn = Cast<IDBAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->CheckHitAttack();
		}
	}
}