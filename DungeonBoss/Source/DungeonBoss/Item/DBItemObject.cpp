// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/DBItemObject.h"
#include "GameData/DBItemSingleton.h"

UDBItemObject::UDBItemObject()
{
	bIsCountableItem = false;
}

void UDBItemObject::MakeEquipItemData(FName ItemID)
{
	EquipItemData = NewObject<UDBEquipItemData>(this, UDBEquipItemData::StaticClass());
	EquipItemData->SetEquipStat(ItemID);

	//UDBItemSingleton::Get().AddEquipItem(ItemID, this);

	if (EquipItemData)
	{
		UE_LOG(LogTemp, Log, TEXT("Success EquipItemData!"));
	}
}

void UDBItemObject::MakeCountableItemData(FName ItemID, int32 ItemCount)
{
	CountableItemData = NewObject<UDBCountableItemData>(this, UDBCountableItemData::StaticClass());
	CountableItemData->SetCountableItemStat(ItemID, ItemCount);
	CountableItemCount = ItemCount;

	//UDBItemSingleton::Get().AddCountableItem(ItemID, this);
	bIsCountableItem = true;
}
