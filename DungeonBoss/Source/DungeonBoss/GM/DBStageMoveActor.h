// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBStageMoveActor.generated.h"

UENUM(BlueprintType)
enum class ENextBossSelect : uint8
{
	Armored_Boss = 0 UMETA(DisplayName = "Armored_Boss"),
};

UCLASS()
class DUNGEONBOSS_API ADBStageMoveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADBStageMoveActor();

protected:
	virtual void BeginPlay() override;

public:
	void MoveFieldWidgetOn();

//Component Section
//Variables
protected:
	class ADBPlayerController* DBPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> PlayerCheckCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UDBMoveFieldWidget> DBMoveFieldWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UDBMoveFieldWidget> DBMoveFieldWidget;

	uint8 bIsRecentlyWidgetCollapsed;

public:
	void LoadWidget(UUserWidget* UIWidget);
	void CollapseWidget();

//Timer Section
//Functions
public:
	void SetNextTickEnableCanWidgetCollapsed();

//CheckPlayer Section
//Variables
protected:
	uint8 bCheckPlayersInArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NextBossSelect)
	ENextBossSelect NextBossSelect;

//Functions
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
