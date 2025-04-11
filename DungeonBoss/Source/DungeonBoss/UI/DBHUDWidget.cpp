// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBHUDWidget.h"
#include "DBHpBarWidget.h"
#include "DBPlayerInteractionWidget.h"
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

void UDBHUDWidget::UpdatePlayerDescription(FString NewDescription)
{
	PlayerInteractDescription->SetVisibility(ESlateVisibility::Visible);
	PlayerInteractDescription->UpdateDescription(NewDescription);
}

void UDBHUDWidget::DisablePlayerDescription()
{
	PlayerInteractDescription->SetVisibility(ESlateVisibility::Collapsed);
}

void UDBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UDBHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	EnemyHpBar = Cast<UDBEnemyHpBarWidget>(GetWidgetFromName(TEXT("WidgetBossHpBar")));
	ensure(EnemyHpBar);
	EnemyHpBar->SetVisibility(ESlateVisibility::Collapsed);

	PlayerInteractDescription = Cast<UDBPlayerInteractionWidget>(GetWidgetFromName(TEXT("PlayerInteractDescription")));
	ensure(PlayerInteractDescription);
	PlayerInteractDescription->SetVisibility(ESlateVisibility::Collapsed);

	IDBCharacterHUDInterface* HUDPawn = Cast<IDBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
