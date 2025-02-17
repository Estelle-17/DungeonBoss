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
	void AddCountableItem(FName ItemID, int ItemCount, bool bIsItemOverlap);

	void SettingEquipItemForPlayer(UDBItemObject* ItemObject, int FromSlotNumber, EEquipItemType FromEquipItemType, EItemSlotType FromItemSlotType);

	UDBCharacterStatComponent* CharacterStat;

protected:

//ItemSection
//Variables
	UPROPERTY(EditAnywhere)
	TArray<class UDBInventoryBlockWidget*> ItemSlots;

//Functions
	//아이템 이동 관련 함수
	void DragSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, int ToItemNumber, EItemSlotType ToItemSlotType);
	void EquipSwapItems(int FromItemNumber, EItemSlotType FromItemSlotType, EEquipItemType EquipItemType, EItemSlotType ToItemSlotType);
	void MoveEquipItemToEmptySlot(UDBItemObject* ItemObject, int ItemSlotNumber);
	int CheckEmptyItemSlot();
	int FindCountableItem(FName NewItemID);
	//캐릭터 스탯 관련 함수
	void SetCharacterItemStat(FDBCharacterStat NewEquipStat, int32 ItemType);
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

// Variables
protected:
	float DivideItemCount;

	UPROPERTY()
	class UDBItemObject* CurrentItemObject;

//Widget
//Variables
public:
	UPROPERTY()
	class UCanvasPanel* ItemCountScrollBarCanvas;

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
