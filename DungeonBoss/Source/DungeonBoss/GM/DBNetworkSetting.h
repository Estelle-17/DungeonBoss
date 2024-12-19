// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DBNetworkSetting.generated.h"

UCLASS()
class DUNGEONBOSS_API ADBNetworkSetting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADBNetworkSetting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void CreateRoomSection();

	UFUNCTION(BlueprintCallable)
	void DestroyRoomSection();

	UFUNCTION(BlueprintCallable)
	void FindRoomSection();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccssful);

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
	TObjectPtr<class UButton> JoinRoomButton;

	UPROPERTY()
	TObjectPtr<class UButton> JoinRandomRoomButton;

	IOnlineSessionPtr OnlineSessionInterface;

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};
