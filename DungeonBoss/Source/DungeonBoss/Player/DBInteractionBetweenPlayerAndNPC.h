// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBPlayerBase.h"
#include "DBInteractionBetweenPlayerAndNPC.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBInteractionBetweenPlayerAndNPC : public UObject
{
	GENERATED_BODY()
	
public:
	UDBInteractionBetweenPlayerAndNPC();
	
protected:
	class ADBNPCBaseActor* NPCActor;
	
public:
	void InteractionNPC();
	void PlayerSearchOverlapSetting(ADBPlayerBase* PlayerBase);

	UFUNCTION()
	void OnSearchOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSearchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
