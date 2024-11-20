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

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

public:
	void SetEquipItem(FName ItemID);

protected:
	UDBEquipItemData* EquipItemData;

	UPROPERTY()
	TObjectPtr<class UTextBlock> ItemCount;

	UPROPERTY()
	TObjectPtr<class UImage> ItemImage;
};
