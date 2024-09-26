// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_EnableComboTime.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UAnimNotifyState_EnableComboTime : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
};
