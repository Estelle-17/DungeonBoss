// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"

ADBPlayerController::ADBPlayerController()
{

}

//��Ʈ��ũ�� �����ϰ� ���͸� �ʱ�ȭ�� �� ���
void ADBPlayerController::PostInitializeComponents()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

//���� Ŭ���̾�Ʈ���� ��Ʈ��ũ�� �ʱ�ȭ�� �ʿ��� ������ ���޹��� ���� ��� �������� �Ǹ� ȣ��
void ADBPlayerController::PostNetInit()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();

	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			DB_LOG(LogDBNetwork, Log, TEXT("Server Connections : %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADBPlayerController::OnPossess(APawn* InPawn)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}
