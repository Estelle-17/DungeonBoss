// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM/DBNetworkSetting.h"
#include "DBMultiUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBMultiUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBMultiUIWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void BindingButtons(ADBNetworkSetting* NetworkSetting);

protected:
	UPROPERTY()
	TObjectPtr<class UCheckBox> PrivateCheck;

	UPROPERTY()
	TObjectPtr<class UEditableTextBox> CreateRoomText;

	UPROPERTY()
	TObjectPtr<class UEditableTextBox> JoinRoomText;

	UPROPERTY()
	TObjectPtr<class UButton> CreateRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> DestroyRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> JoinRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> JoinRandomRoomButton;
};