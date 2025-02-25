// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DBItemSingleton.h"

DEFINE_LOG_CATEGORY(LogDBItemSingleton);

UDBItemSingleton::UDBItemSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> EquipItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBItemStatTable.DBItemStatTable'"));
	if (nullptr != EquipItemDataTableRef.Object)
	{
		EquipItemDataTable = EquipItemDataTableRef.Object;
		check(EquipItemDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CountableItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBMaterialItemStatTable.DBMaterialItemStatTable'"));
	if (nullptr != CountableItemDataTableRef.Object)
	{
		CountableItemDataTable = CountableItemDataTableRef.Object;
		check(CountableItemDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MakeItemMenuTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBMakeItemMenuTable.DBMakeItemMenuTable'"));
	if (nullptr != MakeItemMenuTableRef.Object)
	{
		MakeItemMenuTable = MakeItemMenuTableRef.Object;
		check(MakeItemMenuTable->GetRowMap().Num() > 0);

		MakeItemMenuTable->GetAllRows<FDBMakeItemMenuTable>(TEXT("GetAllRows : MakeItemMenuItems"), MakeItemMenuItems);
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

void UDBItemSingleton::AddEquipItem(FName ItemID, UDBItemObject* ItemObject)
{
	EquipItems.Emplace(ItemObject);
}

void UDBItemSingleton::RemoveEquipItem(UDBItemObject* ItemObject)
{
	EquipItems.Remove(ItemObject);
}

void UDBItemSingleton::AddCountableItem(FName ItemID, UDBItemObject* ItemObject)
{
	CountableItems.Emplace(ItemID, ItemObject);
}
