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
	//현재 아이템의 갯수 저장 및 현재 나눌 아이템의 데이터 저장
	CountSlider->SetMinValue(1);
	CountSlider->SetMaxValue(MaxCount-1);
	CountSlider->SetValue(1);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Min : %f, Max : %f"), CountSlider->GetMinValue(), CountSlider->GetMaxValue()));

	//눈에 보이는 숫자 설정
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), 1.0f)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CountSlider->GetMaxValue())));

	CurrentItemObject = ItemObject;
}

void UDBItemCountScrollBarWidget::DivideItem()
{
	//나누어진 갯수만큼 아이템을 둘로 나누어줌
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

	//눈에 보이는 숫자 설정
	MinCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DivideItemCount)));
	MaxCountText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), (CountSlider->GetMaxValue() - DivideItemCount) + 1)));
}
