// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DBPlayerItemComponent.h"

UDBPlayerItemComponent::UDBPlayerItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UDBPlayerItemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDBPlayerItemComponent::UpdateCountableItem(FName ItemID, int32 ItemCount)
{
	OnAddCountableItem.Broadcast(ItemID, ItemCount);
}

void UDBPlayerItemComponent::UpdateEquipItem(FName ItemID)
{
	OnAddEquipItem.Broadcast(ItemID);
}