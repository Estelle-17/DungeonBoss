// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterMovementComponent.h"

UABCharacterMovementComponent::UABCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	AirControl = 0.35f;
	MaxWalkSpeed = 500.f;
	MinAnalogWalkSpeed = 100.f;
	BrakingDecelerationWalking = 2000.f;
	bRequestedMoveUseAcceleration = true;
	//외부의 물리 반응 끄기
	bEnablePhysicsInteraction = false;
}