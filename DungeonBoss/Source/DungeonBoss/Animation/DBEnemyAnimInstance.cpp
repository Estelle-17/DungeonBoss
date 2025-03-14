// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DBEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/DBEnemyBase.h"

UDBEnemyAnimInstance::UDBEnemyAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UDBEnemyAnimInstance::NativeInitializeAnimation()
{
	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UDBEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Movement)
	{
		Velocity = Movement->Velocity;
		MoveSpeed = Velocity.Size2D();
		bIsIdle = MoveSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
	}
}
