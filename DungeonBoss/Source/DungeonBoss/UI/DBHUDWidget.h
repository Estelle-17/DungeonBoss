// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/DBCharacterStat.h"
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

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UDBHpBarWidget> HpBar;

};
