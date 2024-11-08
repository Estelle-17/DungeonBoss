// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DBPlayerBase.h"
#include "GameFramework/Character.h"
#include <EnhancedInputLibrary.h>
#include "PlayerCharacter.generated.h"

UCLASS()
class DUNGEONBOSS_API APlayerCharacter : public ADBPlayerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//EnhancedInput
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GuardOrDodgeAction;

	void PlayerLook(const FInputActionValue& Value);
	void PlayerMove(const FInputActionValue& Value);
	void PlayerAttack(const FInputActionValue& Value);
	void PlayerGuardOrDodge(const FInputActionValue& Value);

	void PlayComboAttack();
	void PlayGuard();
	void PlayDodge();

	void AttackHitCheck();
	void AttackHitConfirm(AActor* HitActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCUpdateTargetVector(FVector MoveVector);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCGuard(float GuardStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCDodge(float DodgeStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();

	UFUNCTION(Client, Unreliable)
	void ClientRPCUpdateTargetVector(APlayerCharacter* CharacterToPlay, FVector MoveVector);

	UFUNCTION(Client, Unreliable)
	void ClientRPCProcessComboAttack(APlayerCharacter* CharacterToPlay);

	UFUNCTION(Client, Unreliable)
	void ClientRPCProcessGuard(APlayerCharacter* CharacterToPlay);

	UFUNCTION(Client, Unreliable)
	void ClientRPCProcessDodge(APlayerCharacter* CharacterToPlay);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCGuard();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCDodge();

//Notify Section
protected:
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
