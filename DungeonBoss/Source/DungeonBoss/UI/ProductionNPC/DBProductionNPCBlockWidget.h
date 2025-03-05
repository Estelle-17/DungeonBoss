// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DBProductionNPCBlockWidget.generated.h"



/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBProductionNPCBlockWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UDBProductionNPCBlockWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//IUserObjectListEntry
protected:
	//아이템 UI설정 시 호출
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//아이템 선택될 시 호출
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

//Widgets
//Variables
public:
	UPROPERTY()
	class UTextBlock* ItemNameText;

	UPROPERTY()
	class UCanvasPanel* MenuPanel;
public:
	void SetItemUISetting();
//ItemData
protected:
	class UDBMakeItemsData* MakeItemData;
};
