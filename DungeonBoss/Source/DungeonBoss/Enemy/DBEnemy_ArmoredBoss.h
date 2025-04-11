// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DBEnemyMotionWarpingInterface.h"
#include "Interface/DBArmoredBossAttackInterface.h"
#include "DBEnemy_ArmoredBoss.generated.h"

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	SwordAttack UMETA(DisplayName = "SwordAttack"),
	ShieldAttack UMETA(DisplayName = "ShieldAttack"),
	JumpAttack UMETA(DisplayName = "JumpAttack"),
	TurnAttack UMETA(DisplayName = "TurnAttack"),
	WeaponComboAttack UMETA(DisplayName = "WeaponComboAttack"),
	CounterPose UMETA(DisplayName = "CounterPose"),
	CounterAttack UMETA(DisplayName = "CounterAttack"),
	Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class DUNGEONBOSS_API ADBEnemy_ArmoredBoss : public ACharacter, public IDBEnemyMotionWarpingInterface, public IDBArmoredBossAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADBEnemy_ArmoredBoss(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	class UDBEnemyStatComponent* Stat;

//Functions
protected:
	void DeadSetting();

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
public:
	UPROPERTY(ReplicatedUsing = OnRep_AnimationState)
	EAnimationState CurentAnimationState;

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

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CounterActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadActionMontage;

	uint8 bIsCounterState;

//Functions
public:
	UFUNCTION()
	void OnRep_AnimationState();

protected:
	void SetAnimationState(EAnimationState NewState);

	void PlayAttackAction(EAnimationState NewState);

	void PlaySwordAttackAction();
	void PlayShieldAttackAction();
	void PlayTurnAttackAction();
	void PlayTurnToTargetAction();
	void PlayWeaponComboAttackAction();
	void PlayCounterAction();
	void PlayCounterAttackAction();

	void PlayJumpAttackAction();
	
	void PlayDeadAction();

	void AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void TurnActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void DeadActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

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
	virtual void ReleaseWeapons() override;

	virtual void SwordAttackBegin() override;
	virtual void SwordAttackEnd() override;
	virtual void CounterStateBegin() override;
	virtual void CounterStateEnd() override;

//Attack Section
protected:
	TArray<AActor*> DamageHitPlayers;

	TArray<AActor*> CheckHitPlayers;

//Functions
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAArmoredBoss_CooltimeData> ArmoredBoss_CooltimeData;

	void AttackHitConfirm(AActor* HitActor);
};
