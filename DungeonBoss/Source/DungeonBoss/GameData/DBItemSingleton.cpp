// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DBItemSingleton.h"

DEFINE_LOG_CATEGORY(LogDBItemSingleton);

UDBItemSingleton::UDBItemSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBItemStatTable.DBItemStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		ItemDataTable = DataTableRef.Object;
		check(ItemDataTable->GetRowMap().Num() > 0);
	}
}

UDBItemSingleton& UDBItemSingleton::Get()
{
	UDBItemSingleton* Singleton = CastChecked<UDBItemSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogDBItemSingleton, Error, TEXT("Invalid Game Singleton"));

	return *NewObject<UDBItemSingleton>();
}

UDBEquipItemData* UDBItemSingleton::AddEquipItem(FName ItemID)
{
	UDBEquipItemData* EquipItem = NewObject<UDBEquipItemData>(this, UDBEquipItemData::StaticClass());
	EquipItem->SetEquipStat(ItemID);
	EquipItems.Emplace(EquipItem);
	if (EquipItem)
	{
		UE_LOG(LogTemp, Log, TEXT("Complete EquipItem Setting"));
	}
	return EquipItem;
}

void UDBItemSingleton::RemoveEquipItem(UDBEquipItemData* EquipItemData)
{
	EquipItems.Remove(EquipItemData);
}

void UDBItemSingleton::AddCountableItem(FName ItemID)
{

}
