// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM/DBNetworkSetting.h"
#include "DBMultiUIWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinGameServerDelegate, FString /*Address*/);

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

	UFUNCTION(BlueprintCallable)
	void JoinGameServer();

	UPROPERTY()
	TObjectPtr<class UCheckBox> PrivateCheck;

	UPROPERTY()
	TObjectPtr<class UEditableTextBox> CreateRoomText;

	UPROPERTY()
	TObjectPtr<class UEditableTextBox> JoinRoomText;

protected:
	void SetPlayerLocalAddress();

	UFUNCTION(BlueprintCallable)
	void CoverImageOnOff();

	UPROPERTY()
	TObjectPtr<class UButton> CreateRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> DestroyRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> JoinRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> JoinRandomRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> CheckIPButton;

	UPROPERTY()
	TObjectPtr<class UImage> CoverImage;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> AddressText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CheckIPButtonText;

	FOnJoinGameServerDelegate OnJoinGameServerDelegate;

//Input Section
protected:
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};
