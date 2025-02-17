// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Equip/DBEquipItemData.h"
#include "DBPlayerItemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddEquipItemDelegate, FName /*ItemID*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAddCountableItemDelegate, FName /*ItemID*/, int /*ItemCount*/, bool /*bIsItemOverlap*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONBOSS_API UDBPlayerItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBPlayerItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnAddEquipItemDelegate OnAddEquipItem;
	FOnAddCountableItemDelegate OnAddCountableItem;
	void UpdateEquipItem(FName ItemID);
	void UpdateCountableItem(FName ItemID, int ItemCount, bool bIsItemOverlap);
};
