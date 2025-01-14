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

	return EquipItem;
}

void UDBItemSingleton::RemoveEquipItem(UDBEquipItemData* EquipItemData)
{
	EquipItems.Remove(EquipItemData);
}

UDBCountableItemData* UDBItemSingleton::AddCountableItem(FName ItemID, int32 ItemCount)
{
	//�������� ID�� ������ �����ϸ� �̸� �ҷ��� �����ͷ� ���� ������
	if (CountableItems.Contains(ItemID))
	{
		UDBCountableItemData* CountableItem = CountableItems[ItemID];
		CountableItem->SetItemCount(ItemCount);

		return CountableItem;
	}
	else
	{
		UDBCountableItemData* CountableItem = NewObject<UDBCountableItemData>(this, UDBCountableItemData::StaticClass());
		CountableItem->SetCountableItemStat(ItemID, ItemCount);
		CountableItems.Emplace(ItemID, CountableItem);

		return CountableItem;
	}
}
