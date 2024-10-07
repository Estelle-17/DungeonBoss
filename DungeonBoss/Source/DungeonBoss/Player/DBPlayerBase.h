// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DBAnimationAttackInterface.h"
#include "Interface/DBAnimationNotifyInterface.h"
#include "DBPlayerBase.generated.h"

UCLASS()
class DUNGEONBOSS_API ADBPlayerBase : public ACharacter, public IDBAnimationAttackInterface, public IDBAnimationNotifyInterface
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

	void CheckNextAnimation(int32 CheckNumber);
	void MontageAnimationOut();

//IDBAnimationNotifyInterface Section
protected:
	virtual void CheckEnableComboTime() override;
	virtual void EnableGuardTime() override;
	virtual void DisableGuardTime() override;
	virtual void AnimationOutEnable() override;
	virtual void AnimationOutDisable() override;

public:
	UPROPERTY(replicated)
	uint8 bCanAnimationOut = 0;

//IDBAnimationAttackInterface Section
protected:
	virtual void CheckHitAttack() override;
	virtual void NextComboCheck() override;

//ComboAttack Section
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDAComboActionData> ComboActionData;

	void ProcessCombeCommand();
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

	UPROPERTY(ReplicatedUsing = OnRepCheckAttack)
	uint8 HasNextCombo = 0;

	UPROPERTY(ReplicatedUsing = OnRepCheckAttack)
	uint8 bIsAttack : 1;

	UFUNCTION()
	void OnRepCheckAttack();

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
	
public:
	UPROPERTY(replicated)
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

public:
	UPROPERTY(replicated)
	uint8 bIsDodge : 1;

	void DodgeActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
//Attack Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;
};
