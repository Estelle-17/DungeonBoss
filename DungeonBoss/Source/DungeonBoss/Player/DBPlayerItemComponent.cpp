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

void UDBPlayerItemComponent::UpdateInventory(FName ItemID)
{
	OnAddItem.Broadcast(ItemID);
}