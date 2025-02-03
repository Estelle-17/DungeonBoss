// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DBCountableItemData.h"
#include "GameData/DBItemSingleton.h"
#include "DungeonBoss.h"
#include "Engine/Texture2D.h"

void UDBCountableItemData::SetCountableItemStat(FName ItemID, int32 Count)
{
	const FDBMaterialItemStat* DataTable = UDBItemSingleton::Get().GetLoadCountableItemData(ItemID);

	FDBMaterialItemStat Item;

	Item.Name = DataTable->Name;
	Item.ItemType = DataTable->ItemType;
	Item.Rank = DataTable->Rank;
	Item.TexturePath = DataTable->TexturePath;

	ItemTexture = LoadObject<UTexture2D>(nullptr, *Item.TexturePath);

	switch (Item.ItemType)
	{
	case 0:
		CountableItemType = ECountableItemType::Material;
		break;
	}

	SetItemStat(Item);
}
