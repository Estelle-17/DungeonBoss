// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DBEnemyMotionWarpingInterface.h"
#include "DBEnemy_ArmoredBoss.generated.h"

UCLASS()
class DUNGEONBOSS_API ADBEnemy_ArmoredBoss : public ACharacter, public IDBEnemyMotionWarpingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADBEnemy_ArmoredBoss(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

//Component Section
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> ShieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

protected:
	virtual void NotifyComboActionEnd();

//Montage Section
//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SwordAttackActionMontage;

//Functions
protected:
	void PlaySwordAttackAction();
	void AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

//MotionWarping Section
//Variables
public:
	APawn* TargetPawn;

protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	uint8 bCheckMotionWarping = 0;
//Functions
protected:
	virtual void CheckTagetLocationBegin() override;
	virtual void CheckTagetLocationEnd() override;

	void UpdateMotionWarpingRotation(FVector MovementVector);
	void ResetMotionWarpingRotation();
};
