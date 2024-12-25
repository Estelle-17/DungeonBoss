// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/DBMultiSettingActor.h"
#include "Net/UnrealNetwork.h"
#include "DungeonBoss.h"

// Sets default values
ADBMultiSettingActor::ADBMultiSettingActor()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADBMultiSettingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

