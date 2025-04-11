// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBEnemyHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UDBEnemyHpBarWidget::UDBEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = 100.0f;
}

void UDBEnemyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHpBar")));
	ensure(HpProgressBar);
	
	BossNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossNameText")));
	ensure(BossNameText);
}

void UDBEnemyHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UDBEnemyHpBarWidget::UpdateHpBar(float NewHpPercent, FName NewName)
{
	ensure(MaxHp > 0.0f);

	if (HpProgressBar && BossNameText)
	{
		HpProgressBar->SetPercent(NewHpPercent);
		BossNameText->SetText(FText::FromName(NewName));
	}
}