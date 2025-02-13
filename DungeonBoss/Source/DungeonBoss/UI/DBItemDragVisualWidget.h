// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBItemDragVisualWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBItemDragVisualWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBItemDragVisualWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

//Image Setting

//Variables
public:
	UPROPERTY()
	UTexture2D* ItemTexture;

protected:
	UPROPERTY()
	class UImage* ItemImage;

//Functions
public:
	void SetItemImage(FVector2D ImageSize);
};
