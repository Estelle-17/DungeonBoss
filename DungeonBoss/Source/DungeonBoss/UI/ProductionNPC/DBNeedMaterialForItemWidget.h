// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBNeedMaterialForItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBNeedMaterialForItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDBNeedMaterialForItemWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//Widgets
//Variables
protected:
	UPROPERTY()
	class UTextBlock* NeedCountText;

	UPROPERTY()
	class UImage* ItemImage;

public:
	void SetBlockSetting(UTexture2D* NewItemTexture, int CurrenItemCount, int NeedItemCount);

	UFUNCTION()
	void MakeItem(FName ItemID);
};
