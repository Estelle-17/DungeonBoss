// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBPlayerInteractionWidget.h"
#include "Components/TextBlock.h"

UDBPlayerInteractionWidget::UDBPlayerInteractionWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UDBPlayerInteractionWidget::NativeConstruct()
{
	InteractionKey = Cast<UTextBlock>(GetWidgetFromName(TEXT("InteractionKey")));
	ensure(InteractionKey);

	Description = Cast<UTextBlock>(GetWidgetFromName(TEXT("Description")));
	ensure(Description);
}

void UDBPlayerInteractionWidget::UpdateDescription(FString NewDescription)
{
	Description->SetText(FText::FromString(NewDescription));
}
