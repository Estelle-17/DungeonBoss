// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Item/Equip/DBEquipItemData.h"
#include "Item/DBCountableItemData.h"
#include "Item/DBItemObject.h"
#include "Item/DBItemData.h"
#include "DBInventoryBlockWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FOnDragSwapItems, int /*FromItemNumber*/, EItemSlotType /*FromItemSlotType*/, int /*ToItemNumber*/, EItemSlotType /*ToItemSlotType*/)
DECLARE_MULTICAST_DELEGATE_FourParams(FOnEquipItems, UDBItemObject* /*FromItemObject*/, int /*FromItemNumber*/, EEquipItemType /*FromEquipItemType*/, EItemSlotType /*FromItemSlotType*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoveEquipItemToEmptySlot, UDBItemObject* /*ItemObject*/, int /*ItemSlotNumber*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDivideItemSlider, UDBItemObject* /*ItemObject*/, int /*MaxCount*/)

UCLASS()
class DUNGEONBOSS_API UDBInventoryBlockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBInventoryBlockWidget(const FObjectInitializer& ObjectInitializer);


//Variables
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Slot)
	int SlotNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Slot)
	EItemSlotType ItemSlotType;

	UPROPERTY()
	class UDBItemDragVisualWidget* ItemDragVisualWidget;

	UPROPERTY()
	class UDBItemCountScrollBarWidget* ItemCountScrollBarWidget;

	UPROPERTY()
	uint8 bIsEquipSlot;

	class ADBPlayerController* PlayerController;

	FOnDragSwapItems OnDragSwapItems;
	FOnEquipItems OnEquipItems;
	FOnMoveEquipItemToEmptySlot OnMoveEquipItemToEmptySlot;
	FOnDivideItemSlider OnDivideItemSlider;

//Functions
public:
	void SetItemSetting(UObject* ItemObject);
	void SetEquipItemSetting();
	void SetCountableItemSetting();
	void ResetInventorySlot();

	FORCEINLINE UDBItemObject* GetItemObjectData() { return ItemObjectData; }

protected:
	virtual void NativeConstruct() override;

//Widgets
//Variables
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* ItemObjectData;

	UPROPERTY()
	class UTextBlock* ItemCount;

	UPROPERTY()
	class UImage* ItemImage;

	UPROPERTY()
	class UImage* TranslucnetImage;

	UPROPERTY()
	class UImage* ItemEnterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Texture, Meta = (AllowPrivateAccess = "true"))
	UTexture2D* ItemSlotTexture;

//Functions
public:
	void SetTranslucnetImageDisable();

//Input Setting
protected:
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

//DragDrop Setting
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
