// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBHpBarWidget.h"
#include "Components/ProgressBar.h"

UDBHpBarWidget::UDBHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UDBHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHpBar")));

	ensure(HpProgressBar);
}

void UDBHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
