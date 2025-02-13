// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/DBItemData.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int SlotNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EItemSlotType SlotType;
};
