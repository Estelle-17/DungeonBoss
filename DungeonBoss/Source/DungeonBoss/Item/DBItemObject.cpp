// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/DBItemObject.h"
#include "GameData/DBItemSingleton.h"

UDBItemObject::UDBItemObject()
{
	bIsCountableItem = false;
}

void UDBItemObject::MakeEquipItemData(FName NewItemID)
{
	EquipItemData = NewObject<UDBEquipItemData>(this, UDBEquipItemData::StaticClass());
	EquipItemData->SetEquipStat(NewItemID);

	//UDBItemSingleton::Get().AddEquipItem(ItemID, this);
	ItemID = NewItemID;

	if (EquipItemData)
	{
		UE_LOG(LogTemp, Log, TEXT("Success EquipItemData!"));
	}
}

void UDBItemObject::MakeCountableItemData(FName NewItemID, int ItemCount)
{
	CountableItemData = NewObject<UDBCountableItemData>(this, UDBCountableItemData::StaticClass());
	CountableItemData->SetCountableItemStat(NewItemID, ItemCount);
	CountableItemCount = ItemCount;

	ItemID = NewItemID;

	//UDBItemSingleton::Get().AddCountableItem(ItemID, this);
	bIsCountableItem = true;
}
