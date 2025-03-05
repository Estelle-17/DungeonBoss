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
	void SetBlockSetting(FName NewItemID, int NewCurrentItemCount, int NewNeedItemCount);

//Item
//Variables
public:
	int NeedItemCount;
protected:
	FName ItemID;

	int CurrentItemCount;
//Functions
public:
	FORCEINLINE const FName GetItemID() { return ItemID; }
	bool CheckCanCreateItem();
};
