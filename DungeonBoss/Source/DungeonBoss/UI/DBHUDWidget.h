// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/DBCharacterStat.h"
#include "DBEnemyHpBarWidget.h"
#include "DBHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FDBCharacterStat& BaseStat, const FDBCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	void UpdateBossHpBar(float NewCurrentHp, FName BossName);
	void UpdatePlayerDescription(FString NewDescription);
	void DisablePlayerDescription();

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE UDBEnemyHpBarWidget* GetEnemyHpBarWidget() { return EnemyHpBar; }
protected:
	UPROPERTY()
	TObjectPtr<class UDBHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UDBEnemyHpBarWidget> EnemyHpBar;

	UPROPERTY()
	TObjectPtr<class UDBPlayerInteractionWidget> PlayerInteractDescription;

};
