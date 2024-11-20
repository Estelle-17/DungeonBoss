// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/DBGameSingleton.h"

DEFINE_LOG_CATEGORY(LogDBGameSingleton);

UDBGameSingleton::UDBGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBItemStatTable.DBItemStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		ItemDataTable = DataTableRef.Object;
		check(ItemDataTable->GetRowMap().Num() > 0);
	}
}

UDBGameSingleton& UDBGameSingleton::Get()
{
	UDBGameSingleton* Singleton = CastChecked<UDBGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogDBGameSingleton, Error, TEXT("Invalid Game Singleton"));

	return *NewObject<UDBGameSingleton>();
}