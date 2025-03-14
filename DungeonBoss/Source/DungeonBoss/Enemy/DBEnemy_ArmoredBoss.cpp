// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemy_ArmoredBoss.h"
#include "Stat/DBEnemyStatComponent.h"
#include "DungeonBoss.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/DBHpBarWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/DBEnemyMovementComponent.h"
#include "AI/DBAIController.h"
#include "Animation/AnimMontage.h"
#include "MotionWarpingComponent.h"

// Sets default values
ADBEnemy_ArmoredBoss::ADBEnemy_ArmoredBoss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDBEnemyMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(50.0f, 200.0f);
	
	/*GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	Body->SetupAttachment(GetCapsuleComponent());
	Body->SetRelativeLocation(FVector3d(0.0f, 0.0f, -200.0f));
	Body->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));*/
	GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -200.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//SkeletalMesh Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/Boss/Boss.Boss'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}

	//Animation Setting
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/Boss/Armored_Boss/ABP_DB_Armored_Boss.ABP_DB_Armored_Boss_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	//Equip StaticMesh Setting
	/*WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bip001-R-Hand"));
	WeaponMesh->SetRelativeLocation(FVector3d(10.0f, 0.0f, -4.0f));
	WeaponMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, -90.0f));

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bip001-L-Hand"));
	ShieldMesh->SetRelativeLocation(FVector3d(12.0f, -1.0f, -2.0f));
	ShieldMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossSword.BossSword'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh->SetStaticMesh(WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossShield.BossShield'"));
	if (ShieldMeshRef.Object)
	{
		ShieldMesh->SetStaticMesh(ShieldMeshRef.Object);
	}*/

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	//Widget Section
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));

	//Montage Section
	//SwordAttack Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwordAttackActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_SwordAttack.AM_DB_ArmoredBoss_SwordAttack'"));
	if (SwordAttackActionMontageRef.Object)
	{
		SwordAttackActionMontage = SwordAttackActionMontageRef.Object;
	}
}

void ADBEnemy_ArmoredBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ADBEnemy_ArmoredBoss::NotifyComboActionEnd()
{
	
}

void ADBEnemy_ArmoredBoss::PlaySwordAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SwordAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SwordAttackActionMontage);
}

void ADBEnemy_ArmoredBoss::AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	NotifyComboActionEnd();
}

void ADBEnemy_ArmoredBoss::CheckTagetLocationBegin()
{
	bCheckMotionWarping = true;
}

void ADBEnemy_ArmoredBoss::CheckTagetLocationEnd()
{
	bCheckMotionWarping = false;
}

void ADBEnemy_ArmoredBoss::UpdateMotionWarpingRotation(FVector MovementVector)
{
	FMotionWarpingTarget Target;

	if (MovementVector == FVector::Zero())
	{
		Target.Name = TEXT("TurnToTarget");

		Target.Location = GetActorForwardVector();

		//MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
		UE_LOG(LogTemp, Log, TEXT("Check EnemyMotionWarping : %s, %s"), *Target.Name.ToString(), *Target.Rotation.ToString());
	}
	else
	{
		MovementVector.Z = 0.0f;

		Target.Name = TEXT("TurnToTarget");

		Target.Location = MovementVector;

		//FVector TargetLocation = (MovementVector.GetSafeNormal() * 150.0f) + GetActorLocation();
		//FRotator TargetRot = FRotationMatrix::MakeFromX(TargetLocation).Rotator();

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
		//MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("TurnToTarget"), TargetLocation, TargetRot);
		UE_LOG(LogTemp, Log, TEXT("Check EnemyMotionWarping : %s"), *MovementVector.ToString());
	}
	
}

void ADBEnemy_ArmoredBoss::ResetMotionWarpingRotation()
{
}
