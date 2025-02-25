// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/NeedForMakeItemData/DBMakeItemsData.h"

UDBMakeItemsData::UDBMakeItemsData()
{
	bIsChild = false;
}

void UDBMakeItemsData::OnGetItemChildren(UObject* pItem, TArray<UObject*> &aChildren)
{
	aChildren = (TArray<UObject*>)Cast<UDBMakeItemsData>(pItem)->ChildrenItems;
}

void UDBMakeItemsData::SetItemData(FDBMakeItemMenuTable* NewMenuTable)
{
	MenuName = NewMenuTable->Name;
	MenuID = NewMenuTable->MenuID;
	bIsChild = NewMenuTable->IsChild;
	SpacingSize = NewMenuTable->IsChild ? 50 : 0;
}
