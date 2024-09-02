// Fill out your copyright notice in the Description page of Project Settings.


#include "DBGameState.h"
#include "DungeonBoss.h"

void ADBGameState::HandleBeginPlay()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBGameState::OnRep_ReplicatedHasBegunPlay()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}
