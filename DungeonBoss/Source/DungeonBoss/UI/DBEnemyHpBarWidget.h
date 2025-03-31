// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBEnemyHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBEnemyHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDBEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp, FName NewName);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> BossNameText;

	UPROPERTY()
	float MaxHp;
};
