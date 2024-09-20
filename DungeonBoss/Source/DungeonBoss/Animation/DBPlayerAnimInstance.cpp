// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/DBPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerCharacter.h"

UDBPlayerAnimInstance::UDBPlayerAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UDBPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

}

void UDBPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		MoveSpeed = Velocity.Size2D();
		bIsIdle = MoveSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
	}
}
