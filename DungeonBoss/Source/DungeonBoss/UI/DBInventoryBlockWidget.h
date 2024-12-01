// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Item/Equip/DBEquipItemData.h"
#include "DBInventoryBlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBInventoryBlockWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UDBInventoryBlockWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetEquipItemData(UDBEquipItemData* EquipItem) { EquipItemData = EquipItem; }

	FORCEINLINE UDBEquipItemData* GetEquipItemData() { return EquipItemData; }

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	void SetEquipItemSetting(UObject* ListItemObject);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBEquipItemData* EquipItemData;

	UPROPERTY()
	bool bIsCountableItem = 0;

	UPROPERTY()
	class UTextBlock* ItemCount;

	UPROPERTY()
	class UImage* ItemImage;

	uint8 bIsNeedChangeTexture = 0;
};
