// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBMoveFieldWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBMoveFieldWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDBMoveFieldWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//Component
protected:
	UPROPERTY()
	TObjectPtr<class UButton> YesButton;

	UPROPERTY()
	TObjectPtr<class UButton> NoButton;

public:
	class ADBStageMoveActor* StageMoveActor;

//Functions
protected:
	UFUNCTION()
	void MoveField();

	UFUNCTION()
	void DisableWidget();

//Input Section
protected:
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
