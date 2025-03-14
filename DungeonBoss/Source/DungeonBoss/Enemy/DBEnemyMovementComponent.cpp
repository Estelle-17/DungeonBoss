// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemyMovementComponent.h"

UDBEnemyMovementComponent::UDBEnemyMovementComponent()
{
	//�����̴� �������� ȸ�� ����
	bOrientRotationToMovement = false;
	bUseControllerDesiredRotation = false;
	//ȸ�� �ӵ� ����
	RotationRate = FRotator(0.0f, 100.0f, 0.0f);

	AirControl = 0.35f;
	MinAnalogWalkSpeed = 150.f;
	MaxWalkSpeed = 150.0f;
	BrakingDecelerationWalking = 2000.f;
	bRequestedMoveUseAcceleration = true;
}