// Fill out your copyright notice in the Description page of Project Settings.

#include "GameData/DBItemSingleton.h"
#include "GameData/DBMaterialItemStat.h"

DEFINE_LOG_CATEGORY(LogDBItemSingleton);

UDBItemSingleton::UDBItemSingleton()
{
	//게임에 필요한 데이터들 불러오기
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

		TArray<FDBMaterialItemStat*> MaterialItemStatArray;
		CountableItemDataTable->GetAllRows<FDBMaterialItemStat>(TEXT(""), MaterialItemStatArray);
		for (FDBMaterialItemStat* MaterialItemStat : MaterialItemStatArray)
		{
			CountableItems.Emplace(MaterialItemStat->ItemID, 0);
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MakeItemMenuTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBMakeItemMenuTable.DBMakeItemMenuTable'"));
	if (nullptr != MakeItemMenuTableRef.Object)
	{
		MakeItemMenuTable = MakeItemMenuTableRef.Object;
		check(MakeItemMenuTable->GetRowMap().Num() > 0);

		MakeItemMenuTable->GetAllRows<FDBMakeItemMenuTable>(TEXT("GetAllRows : MakeItemMenuItems"), MakeItemMenuItems);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MaterialForMakeItemTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBMaterialForMakeItem.DBMaterialForMakeItem'"));
	if (nullptr != MaterialForMakeItemTableRef.Object)
	{
		MaterialForMakeItemTable = MaterialForMakeItemTableRef.Object;
		check(MaterialForMakeItemTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DBEnemyStatTable.DBEnemyStatTable'"));
	if (nullptr != EnemyStatDataTableRef.Object)
	{
		EnemyStatTable = EnemyStatDataTableRef.Object;
		check(EnemyStatTable->GetRowMap().Num() > 0);

		EnemyStatTable->GetAllRows<FDBEnemyStat>(TEXT("GetAllRows : EnemyStatTableItems"), EnemyStatTableItems);

		for(FDBEnemyStat* EnemyStat : EnemyStatTableItems)
		{
			UE_LOG(LogTemp, Log, TEXT("%f, %f, %f, %f, %f"), EnemyStat->MaxHp, EnemyStat->Attack, EnemyStat->AttackRange, EnemyStat->Defense, EnemyStat->MoveSpeed);
		}
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

void UDBItemSingleton::SetCountableItemObject(FName ItemID, int ItemCount)
{
	int* CurrentItemCount = CountableItems.Find(ItemID);
	int NewItemCount = ItemCount + *CurrentItemCount;
	if (CurrentItemCount)
	{
		UE_LOG(LogTemp, Log, TEXT("CountableItem : %d"), NewItemCount);
		CountableItems.Emplace(ItemID, NewItemCount);
	}
}
