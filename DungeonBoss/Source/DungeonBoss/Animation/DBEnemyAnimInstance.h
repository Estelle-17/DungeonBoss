// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DBEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONBOSS_API UDBEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UDBEnemyAnimInstance();

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling : 1;

};
