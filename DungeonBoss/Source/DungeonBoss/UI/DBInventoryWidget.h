// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Stat/DBCharacterStatComponent.h"
#include "GameData/DBCharacterEquipItems.h"
#include "DBInventoryWidget.generated.h"

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
	void AddCountableItem(FName ItemID, int32 ItemCount);

	void SettingEquipItemForPlayer(UDBItemObject* ItemObject);

	UDBCharacterStatComponent* CharacterStat;

protected:

//ItemSection
	UFUNCTION(BlueprintCallable)
	void InventoryItemClicked(UObject* Item);

	UPROPERTY(EditAnywhere)
	TArray<class UDBInventoryBlockWidget*> ItemSlots;

	int32 CanInputItemSlotIndex;

//CharacterEquipItems
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterEquipItems CharacterEquipStats;

	UPROPERTY()
	TObjectPtr<class UImage> HeadImage;

	UPROPERTY()
	TObjectPtr<class UImage> BodyImage;

	UPROPERTY()
	TObjectPtr<class UImage> ShoesImage;

	UPROPERTY()
	TObjectPtr<class UImage> WeaponImage;
};
