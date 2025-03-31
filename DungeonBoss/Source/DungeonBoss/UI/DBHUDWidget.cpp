// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBHUDWidget.h"
#include "DBHpBarWidget.h"
#include "DBEnemyHpBarWidget.h"
#include "Interface/DBCharacterHUDInterface.h"

UDBHUDWidget::UDBHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UDBHUDWidget::UpdateStat(const FDBCharacterStat& BaseStat, const FDBCharacterStat& ModifierStat)
{
	FDBCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);
}

void UDBHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UDBHUDWidget::UpdateBossHpBar(float NewCurrentHp, FName BossName)
{
	EnemyHpBar->UpdateHpBar(NewCurrentHp, BossName);
}

void UDBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UDBHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	EnemyHpBar = Cast<UDBEnemyHpBarWidget>(GetWidgetFromName(TEXT("WidgetBossHpBar")));
	ensure(EnemyHpBar);

	IDBCharacterHUDInterface* HUDPawn = Cast<IDBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
