// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBNPCBaseActor.generated.h"

UENUM(BlueprintType)
enum class ENPCType : uint8
{
	MultiplayerNPC UMETA(DisplayName = "MultiplayerNPC"),
	ItemMakingNPC UMETA(DisplayName = "ItemMakingNPC")
};

UCLASS()
class DUNGEONBOSS_API ADBNPCBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADBNPCBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	class ADBPlayerController* DBPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType NPCType;

//Timer Section
//Functions
public:
	void SetNextTickEnableCanWidgetCollapsed();

//UI Section
public:
	uint8 bIsPlayerInArea;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBMultiUIWidget> DBMultiUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBMultiUIWidget> DBMultiUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBProductionNPCWidget> DBProductionNPCWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBProductionNPCWidget> DBProductionNPCWidget;

	uint8 bIsRecentlyWidgetCollapsed;

public:
	void LoadWidget(UUserWidget* UIWidget);
	void CollapseWidget();
	void PlayerInteractionAction();

};
