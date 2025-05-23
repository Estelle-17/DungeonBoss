// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DBAnimationAttackInterface.h"
#include "Interface/DBAnimationNotifyInterface.h"
#include "Interface/DBCharacterHUDInterface.h"
#include "Interface/DBMotionWarpingInterface.h"
#include "GameData/DBCharacterStat.h"
#include "Stat/DBCharacterStatComponent.h"
#include "DBPlayerBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateEnemyHpBar, float /*EnemyHpPercnet*/, FName /*EnemyName*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdatePlayerDescription, FString /*NewDescription*/);
DECLARE_MULTICAST_DELEGATE(FOnDisablePlayerDescription);

UCLASS()
class DUNGEONBOSS_API ADBPlayerBase : public ACharacter, public IDBAnimationNotifyInterface, public IDBAnimationAttackInterface,
	public IDBCharacterHUDInterface, public IDBMotionWarpingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBPlayerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;

//MotionWarping Section
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	void SetMotionWarpingRotation(FVector MovementVector);
	void ResetMotionWarpingRotation(); 

	FVector TargetVector;

	uint8 bCheckMotionWarping = 0;

//Stat Section
public:
	FORCEINLINE UDBCharacterStatComponent* GetCharacterStatComponent() { return Stat; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDBCharacterStatComponent> Stat;

//Enemy Section
protected:
	FOnUpdateEnemyHpBar OnUpdateEnemyHpBar;

//Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDBPlayerItemComponent> Inventory;

//UI Section
//Variables
public:
	FOnUpdatePlayerDescription OnUpdatePlayerDescription;
	FOnDisablePlayerDescription OnDisablePlayerDescription;

//Functions
protected:
	virtual void SetupHUDWidget(class UDBHUDWidget* InHUDWidget) override;
	virtual void SetupInventoryWidget(class UDBInventoryWidget* InInventoryWidget) override;

//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	class UDBEnemyHpBarWidget* EnemyHpBar;

//IDBAnimationAttackInterface Section
protected:
	virtual void CheckHitEnable() override;
	virtual void CheckHitDisable() override;

//IDBAnimationNotifyInterface Section
protected:
	virtual void CheckEnableComboTime() override;
	virtual void EnableGuardTime() override;
	virtual void DisableGuardTime() override;
	virtual void AnimationOutEnable() override;
	virtual void AnimationOutDisable() override;
	virtual void DamageImmunityEnable() override;
	virtual void DamageImmunityDisable() override;

//IDBMotionWarpingInterface Section
protected:
	virtual void UpdateMotionWarpingTargetVector() override;
public:
	uint8 bCanAnimationOut = 0;

//NPC Interaction Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InteractionBetweenPlayerAndNPC)
	TObjectPtr<class UDBInteractionBetweenPlayerAndNPC> DBInteractionBetweenPlayerAndNPC;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StageMoveActor)
	class ADBStageMoveActor* DBStageMoveActor;

//Collider Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USphereComponent> SearchCollision;

//private:
//	UFUNCTION()
//	void OnSearchOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	UFUNCTION()
//	void OnSearchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	void TurnToInputVector(FVector NewInputVector);

	void CheckNextAnimation(int32 CheckNumber);
	void AllTimerHandlerStop();
	void SetPlayerStateToIdle();
	void MontageAnimationOut();
//ComboAttack Section
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAComboActionData> ComboActionData;

	void ProcessCombeStartCommand();
	void ProcessNextCombeCommand();

	void JumpToComboAction();
	void ComboActionBegin();
	void SetComboCheckTimer();
	void ComboCheck();

	FTimerHandle EnableComboTimerHandle;
	FTimerHandle NextComboTimerHandle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 MaxCombo;

	uint8 HasNextCombo = 0;
	uint8 bIsAttack : 1;

	UFUNCTION()
	void OnRepCheckAttack();

	float AttackTime = 0.0f;
	float LastAttackStartTime = 0.0f;
	float AttackTimeDifference = 0.0f;
	float AcceptCheckDistance = 300.0f;

public:
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Guard Section
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GuardActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAGuardActionData> GuardActionData;

	void ProcessGuardCommand();
	void GuardActionBegin();
	void SuccessGuard(AActor* NewTarget);
	void CounterAttackAction();

	float GuardTime = 0.0f;
	float LastGuardStartTime = 0.0f;
	float GuardTimeDifference = 0.0f;
	float LastCounterAttackStartTime = 0.0f;
	float GuardAttackTimeDifference = 0.0f;
	
public:
	uint8 bIsGuard : 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	uint8 bCanCounterAttack = 0;

	//true동안 공격 방어
	uint8 bIsGuardState = 0;

	void GuardActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Dodge Section
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DodgeActionMontage;

	void ProcessDodgeCommand();
	void DodgeActionBegin();
	void SetDodgeCheckTimer();
	void DodgeTimeEnd();

	float DodgeTime = 5.0f;

	FTimerHandle DodgeTimerHandle;

	float LastDodgeStartTime = 0.0f;
	float DodgeTimeDifference = 0.0f;

public:
	uint8 bIsDodge : 1;

	void DodgeActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//ChargeAttack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ChargeAttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAChargeAttackActionData> ChargeAttackActionData;

	void ProcessChargeAttackCommand();
	void ChargingActionBegin();
	void ChargeAttackBegin();
	void SetChargeCheckTimer();
	void ChargeCheck();

	FTimerHandle ChargeTimerHandle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentChargeStack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	int32 MaxChargeStack;

	float ChargingTime = 0.0f;
	float LastChargeAttackStartTime = 0.0f;
	float ChargeAttackTimeDifference = 0.0f;
	
	uint8 bIsFullCharge = 0;
	uint8 bIsChargeAttack = 0;
	uint8 bIsCharging = 0;

public:
	void ChargeAttackActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Hit Damage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DamageReceiveActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DamageReceiveDownActionMontage;

	FTimerHandle DamageReceiveTimerHandle;

	float LastDamageReceiveStartTime = 0.0f;
	float DamageReceiveTimeDifference = 0.0f;
	float RecentlyDamageReceiveCheckTime = 5.0f;

	uint8 bIsPlayingDamageReceiveAction = 0;
	uint8 bRecentlyDamageReceive = 0;

protected:
	void ProcessDamageReceiveCommand(AActor* NewTarget);
	void DamageReceiveActionBegin(AActor* NewTarget);
	void DamageReceiveDownActionBegin(AActor* NewTarget);
	void SetDamageReceiveCheckTimer();
	void DamageReceiveTimeEnd();

public:
	void DamageReceiveActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Attack Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	uint8 bIsDamageImmunity;

protected:
	TArray<AActor*> HitEnemies;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
