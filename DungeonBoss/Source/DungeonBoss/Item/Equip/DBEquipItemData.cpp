// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DBEquipItemData.h"
#include "GameData/DBItemSingleton.h"
#include "DungeonBoss.h"
#include "Engine/Texture2D.h"

void UDBEquipItemData::SetEquipStat(FName ItemID)
{
	const FDBItemStat* DataTable = UDBItemSingleton::Get().GetLoadEquipItemData(ItemID);

	FDBItemStat Item;

	Item.Name = DataTable->Name;
	Item.ItemType = DataTable->ItemType;
	Item.Rank = DataTable->Rank;
	Item.MaxHp = DataTable->MaxHp;
	Item.Attack = DataTable->Attack;
	Item.Defense = DataTable->Defense;
	Item.MoveSpeed = DataTable->MoveSpeed;
	Item.TexturePath = DataTable->TexturePath;

	ItemTexture = LoadObject<UTexture2D>(nullptr, *Item.TexturePath);

	switch (Item.ItemType)
	{
		case 0:
			EquipItemType = EEquipItemType::Weapon;
			break;
		case 1:
			EquipItemType = EEquipItemType::Head;
			break;
		case 2:
			EquipItemType = EEquipItemType::Body;
			break;
		case 3:
			EquipItemType = EEquipItemType::Shoes;
			break;
	}

	SetItemStat(Item);

	SetEquipCharacterStat();
}

void UDBEquipItemData::SetEquipCharacterStat()
{
	FDBCharacterStat CharacterStat;

	CharacterStat.MaxHp = ItemStat.MaxHp;
	CharacterStat.Attack = ItemStat.Attack;
	CharacterStat.Defense = ItemStat.Defense;
	CharacterStat.MoveSpeed = ItemStat.MoveSpeed;

	SetCharacterStat(CharacterStat);
}

void UDBEquipItemData::SendEquipItem()
{
	//InventoryWidget->SettingEquipItemForPlayer(this);
}