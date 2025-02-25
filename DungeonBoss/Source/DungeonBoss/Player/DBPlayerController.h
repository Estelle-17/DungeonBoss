// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputLibrary.h"
#include "GM/DBNetworkSetting.h"
#include "DBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API ADBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADBPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

//Binding Section
//Variables
public:
	uint8 bIsRecentlyWidgetCollapsed;
//Functions
public:
	virtual void SetupInputComponent() override;

public:
	FORCEINLINE void SetMultiUIWidgetEnable() { bIsCanMultiUIWidgetOn = true; }
	void SetMultiUIWidgetDisable();

//Timer Section
//Functions
public:
	void SetNextTickEnableCanWidgetCollapsed();

//HUD Section
//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBHUDWidget> DBHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBHUDWidget> DBHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBInventoryWidget> DBInventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBInventoryWidget> DBInventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBMultiUIWidget> DBMultiUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBMultiUIWidget> DBMultiUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBItemDragVisualWidget> DBItemDragVisualWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBItemDragVisualWidget> DBItemDragVisualWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBItemCountScrollBarWidget> DBItemCountScrollBarWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBItemCountScrollBarWidget> DBItemCountScrollBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBEquipNPCWidget> DBEquipNPCWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBEquipNPCWidget> DBEquipNPCWidget;

	bool bIsCanMultiUIWidgetOn;
	bool bIsCanEquipNPCWidgetOn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DivideItemAction;

//Functions
public:
	FORCEINLINE UDBItemDragVisualWidget* GetItemDragVisualWidget() { return DBItemDragVisualWidget; }
	FORCEINLINE UDBItemCountScrollBarWidget* GetItemCountScrollBarWidget() { return DBItemCountScrollBarWidget; }
	void LoadWidget(UUserWidget* UIWidget);
	void CollapseWidget(FString WidgetName);
	bool CheckWidgetVisible();


protected:
	void PlayerInventoryAction(const FInputActionValue& Value);
	void PlayerInteractionAction(const FInputActionValue& Value);

//Network Section
protected:
	UPROPERTY()
	TObjectPtr<ADBNetworkSetting> NetworkSetting;

};
