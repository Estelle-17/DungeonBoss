// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void AddEquipItem(FName ItemID);

protected:
//TileView Section
	UPROPERTY()
	TObjectPtr<class UTileView> WeaponItemBlocks;

	UPROPERTY()
	TObjectPtr<class UTileView> HeadItemBlocks;

	UPROPERTY()
	TObjectPtr<class UTileView> BodyItemBlocks;

	UPROPERTY()
	TObjectPtr<class UTileView> ShoesItemBlocks;

	UPROPERTY()
	TObjectPtr<class UTileView> CountableItemBlocks;

//Button Section
	UPROPERTY()
	TObjectPtr<class UButton> WeaponItemButton;

	UPROPERTY()
	TObjectPtr<class UButton> HeadItemButton;

	UPROPERTY()
	TObjectPtr<class UButton> BodyItemButton;

	UPROPERTY()
	TObjectPtr<class UButton> ShoesItemButton;

	UPROPERTY()
	TObjectPtr<class UButton> CountableItemButton;

	UFUNCTION(BlueprintCallable)
	void WeaponItemButtonCallback();

	UFUNCTION(BlueprintCallable)
	void HeadItemButtonCallback();

	UFUNCTION(BlueprintCallable)
	void BodyItemButtonCallback();

	UFUNCTION(BlueprintCallable)
	void ShoesItemButtonCallback();

	UFUNCTION(BlueprintCallable)
	void CountableItemButtonCallback();

	void SetAllInventoryCollapsed();
};
