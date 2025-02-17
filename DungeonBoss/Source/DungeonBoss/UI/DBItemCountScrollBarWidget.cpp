// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBItemCountScrollBarWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Item/DBItemObject.h"
#include "DBInventoryWidget.h"

UDBItemCountScrollBarWidget::UDBItemCountScrollBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBItemCountScrollBarWidget::NativeConstruct()
{
	CountSlider = Cast<USlider>(GetWidgetFromName(TEXT("CountSlider")));
	ensure(CountSlider);
	CountSlider->OnValueChanged.AddDynamic(this, &UDBItemCountScrollBarWidget::ChangeCountText);

	MinCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MinCountText")));
	ensure(MinCountText);

	MaxCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxCountText")));
	ensure(MaxCountText);

	OKButton = Cast<UButton>(GetWidgetFromName(TEXT("OKButton")));
	ensure(OKButton);
	OKButton->OnClicked.AddDynamic(this, &UDBItemCountScrollBarWidget::DivideItem);

	DivideItemCount = 1;
}

void UDBItemCountScrollBarWidget::SetSliderSetting(UDBItemObject* ItemObject, int MaxCount)
{
	//���� �������� ���� ���� �� ���� ���� �������� ������ ����
	CountSlider->SetMinValue(1);
	CountSlider->SetMaxValue(MaxCount-1);
	CountSlider->SetValue(1);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Min : %f, Max : %f"), CountSlider->GetMinValue(), CountSlider->GetMaxValue()));

	//���� ���̴� ���� ����
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), 1.0f)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CountSlider->GetMaxValue())));

	CurrentItemObject = ItemObject;
}

void UDBItemCountScrollBarWidget::DivideItem()
{
	//�������� ������ŭ �������� �ѷ� ��������
	CurrentItemObject->SetItemCount(int(-DivideItemCount));

	if (DBInventoryWidget)
	{
		DBInventoryWidget->AddCountableItem(CurrentItemObject->ItemID, int(DivideItemCount), false);
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UDBItemCountScrollBarWidget::ChangeCountText(float Value)
{
	DivideItemCount = CountSlider->GetValue();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Value : %.0f"), DivideItemCount));

	//���� ���̴� ���� ����
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DivideItemCount)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), (CountSlider->GetMaxValue() - DivideItemCount) + 1)));
}
