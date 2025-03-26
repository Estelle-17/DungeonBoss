// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DBEnemyMotionWarpingInterface.h"
#include "Interface/DBArmoredBossAttackInterface.h"
#include "DBEnemy_ArmoredBoss.generated.h"

UCLASS()
class DUNGEONBOSS_API ADBEnemy_ArmoredBoss : public ACharacter, public IDBEnemyMotionWarpingInterface, public IDBArmoredBossAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADBEnemy_ArmoredBoss(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	class UDBEnemyStatComponent* Stat;

//Component Section
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> ShieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> ShieldCollision;

protected:
	virtual void NotifyComboActionEnd();
	virtual void NotifyTurnActionEnd();

//Montage Section
//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SwordAttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ShieldAttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> LeftTurnActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RightTurnActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> JumpAttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> TurnAttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> WeaponComboAttackActionMontage;

//Functions
protected:
	void PlayAttackAction(FString NewName);

	void PlaySwordAttackAction();
	void PlayShieldAttackAction();
	void PlayTurnAttackAction();
	void PlayTurnToTargetAction();
	void PlayWeaponComboAttackAction();

	void PlayJumpAttackAction();

	void AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void TurnActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

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

	virtual void EnemyJumpStateBegin() override;
	virtual void EnemyJumpStateEnd() override;

	void UpdateMotionWarpingRotation(FVector MovementVector);
	void ResetMotionWarpingRotation();

//IDBArmoredBossAttackInterface Section
protected:
	virtual void SwordAttackCheck() override;
	virtual void ShieldAttackCheck() override;
	virtual void AroundAttackCheck() override;
	virtual void ForwardAttackCheck() override;
	virtual void ResetHitPlayers() override;

//Attack Section
protected:
	TArray<AActor*> HitPlayers;

//Functions
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAArmoredBoss_CooltimeData> ArmoredBoss_CooltimeData;

	void AttackHitConfirm(AActor* HitActor);

};
