// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DBEquipItemData.h"
#include "GameData/DBGameSingleton.h"

void UDBEquipItemData::SetEquipStat(FName ItemID)
{
	const FDBItemStat* DataTable = UDBGameSingleton::Get().GetItemStat(ItemID);

	ItemStat.Name = DataTable->Name;
	ItemStat.Rank = DataTable->Rank;
	ItemStat.MaxHp = DataTable->MaxHp;
	ItemStat.Attack = DataTable->Attack;
	ItemStat.Defense = DataTable->Defense;
	ItemStat.MovementSpeed = DataTable->MovementSpeed;

	SetEquipCharacterStat();
}

void UDBEquipItemData::SetEquipCharacterStat()
{
	EquipCharacterStat.MaxHp = ItemStat.MaxHp;
	EquipCharacterStat.Attack = ItemStat.Attack;
	EquipCharacterStat.Defense = ItemStat.Defense;
	EquipCharacterStat.MovementSpeed = ItemStat.MovementSpeed;
}
