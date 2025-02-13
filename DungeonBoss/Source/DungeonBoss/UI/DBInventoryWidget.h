// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Stat/DBCharacterStatComponent.h"
#include "GameData/DBCharacterEquipItems.h"
#include "Item/DBItemData.h"
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

	void SettingEquipItemForPlayer(UDBItemObject* ItemObject, int FromSlotNumber, EEquipItemType FromEquipItemType, EItemSlotType FromItemSlotType);

	UDBCharacterStatComponent* CharacterStat;

protected:

//ItemSection
//Variables
	UPROPERTY(EditAnywhere)
	TArray<class UDBInventoryBlockWidget*> ItemSlots;

	int32 CanInputItemSlotIndex;

//Functions
	void DragSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, int ToItemNumber, EItemSlotType ToItemSlotType);
	void EquipSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, EEquipItemType EquipItemType, EItemSlotType ToItemSlotType);
	void SetCharacterItemStat(FDBCharacterStat NewEquipStat, int32 ItemType);
	void MoveEquipItemToEmptySlot(UDBItemObject* ItemObject, int ItemSlotNumber);
	void SettingCharacterEquipStats(UDBItemObject* ItemObject, int ItemSlotNumber);

	UFUNCTION(BlueprintCallable)
	void InventoryItemClicked(UObject* Item);

//CharacterEquipItems
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterEquipItems CharacterEquipStats;

	UPROPERTY()
	TObjectPtr<class UDBInventoryBlockWidget> EquipHeadItem;

	UPROPERTY()
	TObjectPtr<class UDBInventoryBlockWidget> EquipBodyItem;

	UPROPERTY()
	TObjectPtr<class UDBInventoryBlockWidget> EquipWeaponItem;

	UPROPERTY()
	TObjectPtr<class UDBInventoryBlockWidget> EquipShoesItem;
};
