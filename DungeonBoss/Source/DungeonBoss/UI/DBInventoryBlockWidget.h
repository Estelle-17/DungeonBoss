// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Item/Equip/DBEquipItemData.h"
#include "Item/DBCountableItemData.h"
#include "Item/DBItemObject.h"
#include "DBInventoryBlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBInventoryBlockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBInventoryBlockWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetItemSetting(UObject* ItemObject);
	void SetEquipItemSetting();
	void SetCountableItemSetting();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* ItemObjectData;

	UPROPERTY()
	class UTextBlock* ItemCount;

	UPROPERTY()
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Texture)
	UTexture2D* ItemSlotTexture;

//Input Setting
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
