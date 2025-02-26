// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBItemCountScrollBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemCountScrollBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBItemCountScrollBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//Variables
public:
	UPROPERTY()
	class UDBInventoryWidget* DBInventoryWidget;

protected:
	float DivideItemCount;

	UPROPERTY()
	class UDBItemObject* CurrentItemObject;

//Widget
//Variables
protected:
	UPROPERTY()
	class USlider* CountSlider;

	UPROPERTY()
	class UTextBlock* MinCountText;

	UPROPERTY()
	class UTextBlock* MaxCountText;

	UPROPERTY()
	class UButton* OKButton;
	
//Function
public:
	void SetSliderSetting(UDBItemObject* ItemObject, int MaxCount);

protected:
	UFUNCTION()
	void DivideItem();

//Slider Setting
public:
	UFUNCTION()
	void ChangeCountText(float Value);
};
