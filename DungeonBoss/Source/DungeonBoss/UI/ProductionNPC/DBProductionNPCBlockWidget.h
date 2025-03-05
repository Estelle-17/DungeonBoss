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
	//������ UI���� �� ȣ��
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//������ ���õ� �� ȣ��
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
