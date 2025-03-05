// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemyBase.h"
#include "Stat/DBEnemyStatComponent.h"
#include "DungeonBoss.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/DBHpBarWidget.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ADBEnemyBase::ADBEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	SetRootComponent(Capsule);
	
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Capsule);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(Body, TEXT("Bip001-R-Hand"));
	WeaponMesh->SetRelativeLocation(FVector3d(10.0f, 0.0f, -4.0f));
	WeaponMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, -90.0f));

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(Body, TEXT("Bip001-L-Hand"));
	ShieldMesh->SetRelativeLocation(FVector3d(12.0f, -1.0f, -2.0f));
	ShieldMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 90.0f));

	static ConstructorHelpers::FObjectFinder<UCapsuleComponent> CapsuleComponentRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Capsule.Capsule'"));
	if (CapsuleComponentRef.Object)
	{
		Capsule = CapsuleComponentRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/Boss/Boss.Boss'"));
	if (BodyMeshRef.Object)
	{
		Body->SetSkeletalMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossSword.BossSword'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh->SetStaticMesh(WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossShield.BossShield'"));
	if (ShieldMeshRef.Object)
	{
		ShieldMesh->SetStaticMesh(ShieldMeshRef.Object);
	}

	Capsule->SetCapsuleHalfHeight(0);
	Capsule->SetCapsuleRadius(0);

	Body->SetGenerateOverlapEvents(true);
	Body->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	Tags.Add(FName("Enemy"));

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
