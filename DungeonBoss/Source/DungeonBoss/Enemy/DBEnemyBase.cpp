// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemyBase.h"
#include "Stat/DBEnemyStatComponent.h"
#include "DungeonBoss.h"
#include "Components/WidgetComponent.h"
#include "UI/DBHpBarWidget.h"

// Sets default values
ADBEnemyBase::ADBEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	Body->SetGenerateOverlapEvents(true);
	Body->SetRelativeScale3D(FVector3d(2.0f, 2.0f, 2.0f));
	Body->ComponentTags.Add(FName("Enemy"));

	//Stat Section
	Stat = CreateDefaultSubobject<UDBEnemyStatComponent>(TEXT("Stat"));

	//Widget Section
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
}

// Called when the game starts or when spawned
void ADBEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ADBEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADBEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}
