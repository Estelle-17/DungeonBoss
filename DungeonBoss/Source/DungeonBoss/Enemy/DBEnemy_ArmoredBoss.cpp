// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemy_ArmoredBoss.h"
#include "Stat/DBEnemyStatComponent.h"
#include "DungeonBoss.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "UI/DBHpBarWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/DBEnemyMovementComponent.h"
#include "AI/DBAIController.h"
#include "Animation/AnimMontage.h"
#include "MotionWarpingComponent.h"
#include "GameData/DAArmoredBoss_CooltimeData.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADBEnemy_ArmoredBoss::ADBEnemy_ArmoredBoss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDBEnemyMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.05f;

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
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bip001-R-Hand"));
	WeaponMesh->SetRelativeLocation(FVector3d(10.0f, 0.0f, -4.0f));
	WeaponMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, -90.0f));

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bip001-L-Hand"));
	ShieldMesh->SetRelativeLocation(FVector3d(12.0f, -1.0f, -2.0f));
	ShieldMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossSword.BossSword'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShieldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Prefab/Boss/BossShield.BossShield'"));
	if (ShieldMeshRef.Object)
	{
		ShieldMesh->SetSkeletalMesh(ShieldMeshRef.Object);
	}

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	//Collision Setting
	//Sword
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetRelativeLocation(FVector3d(0.0f, 85.0f, 0.0f));
	WeaponCollision->SetRelativeScale3D(FVector3d(0.2f, 3.3f, 0.4f));
	WeaponCollision->SetGenerateOverlapEvents(true);
	WeaponCollision->SetCollisionProfileName(TEXT("EnemyWeapon"));
	
	//Shield
	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(ShieldMesh);
	ShieldCollision->SetRelativeLocation(FVector3d(25.0f, -35.0f, 25.0f));
	ShieldCollision->SetRelativeScale3D(FVector3d(0.35f, 3.3f, 1.5f));
	ShieldCollision->SetGenerateOverlapEvents(true);
	ShieldCollision->SetCollisionProfileName(TEXT("EnemyWeapon"));

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
	//ShieldAttack Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShieldAttackActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_ShieldAttack.AM_DB_ArmoredBoss_ShieldAttack'"));
	if (ShieldAttackActionMontageRef.Object)
	{
		ShieldAttackActionMontage = ShieldAttackActionMontageRef.Object;
	}
	//ShieldAttack Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> TurnAttackActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_TurnAttack.AM_DB_ArmoredBoss_TurnAttack'"));
	if (TurnAttackActionMontageRef.Object)
	{
		TurnAttackActionMontage = TurnAttackActionMontageRef.Object;
	}
	//Turn Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LeftTurnActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_LeftTurn.AM_DB_ArmoredBoss_LeftTurn'"));
	if (LeftTurnActionMontageRef.Object)
	{
		LeftTurnActionMontage = LeftTurnActionMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RightTurnActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_RightTurn.AM_DB_ArmoredBoss_RightTurn'"));
	if (RightTurnActionMontageRef.Object)
	{
		RightTurnActionMontage = RightTurnActionMontageRef.Object;
	}
	//JumpAttack Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpAttackActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_JumpAttack.AM_DB_ArmoredBoss_JumpAttack'"));
	if (JumpAttackActionMontageRef.Object)
	{
		JumpAttackActionMontage = JumpAttackActionMontageRef.Object;
	}
	//WeaponComboAttack Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponComboAttackActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Boss/Armored_Boss/AM_DB_ArmoredBoss_WeaponComboAttack.AM_DB_ArmoredBoss_WeaponComboAttack'"));
	if (WeaponComboAttackActionMontageRef.Object)
	{
		WeaponComboAttackActionMontage = WeaponComboAttackActionMontageRef.Object;
	}

	//Data Section
	static ConstructorHelpers::FObjectFinder<UDAArmoredBoss_CooltimeData> ArmoredBoss_CooltimeDataRef(TEXT("/Script/DungeonBoss.DAArmoredBoss_CooltimeData'/Game/GameData/DA_ArmoredBoss_Cooltime.DA_ArmoredBoss_Cooltime'"));
	if (ArmoredBoss_CooltimeDataRef.Object)
	{
		ArmoredBoss_CooltimeData = ArmoredBoss_CooltimeDataRef.Object;
	}
}

void ADBEnemy_ArmoredBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ADBEnemy_ArmoredBoss::NotifyComboActionEnd()
{
	
}

void ADBEnemy_ArmoredBoss::NotifyTurnActionEnd()
{
}

void ADBEnemy_ArmoredBoss::PlayAttackAction(FString NewName)
{
	//입력된 값에 맞는 애니메이션 출력
	if (NewName.Equals("SwordAttack"))
	{
		PlaySwordAttackAction();
	}
	else if (NewName.Equals("ShieldAttack"))
	{
		PlayShieldAttackAction();
	}
	else if (NewName.Equals("JumpAttack"))
	{
		PlayJumpAttackAction();
	}
	else if (NewName.Equals("TurnAttack"))
	{
		PlayTurnAttackAction();
	}
	else if (NewName.Equals("WeaponComboAttack"))
	{
		PlayWeaponComboAttackAction();
	}
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

void ADBEnemy_ArmoredBoss::PlayShieldAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ShieldAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ShieldAttackActionMontage);
}

void ADBEnemy_ArmoredBoss::PlayTurnAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(TurnAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, TurnAttackActionMontage);
}

void ADBEnemy_ArmoredBoss::PlayWeaponComboAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(WeaponComboAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, WeaponComboAttackActionMontage);
}

void ADBEnemy_ArmoredBoss::PlayTurnToTargetAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (TargetPawn)
	{
		FVector ControllingPawnForwardVector = GetActorForwardVector();
		ControllingPawnForwardVector.Z = 0;
		ControllingPawnForwardVector.Normalize();

		FVector TargetVector = (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		TargetVector.Z = 0;
		//TargetVector.Normalize();

		//UE_LOG(LogTemp, Log, TEXT("AIPawn : %s, Target : %s"), *ControllingPawnForwardVector.ToString(), *TargetVector.ToString());

		//타겟과 적 사이의 각을 알기 위해 두 벡터의 내적 계산
		float Dot = FVector::DotProduct(ControllingPawnForwardVector, TargetVector);
		float AcosAngle = FMath::Acos(Dot);	//Dot한 값을 아크코사인 계산을 진행하면 0~180도 사이의 값(0~1)의 양수 값만 나옴
		float TargetAngle = FMath::RadiansToDegrees(AcosAngle);	

		//타겟이 왼쪽, 오른쪽에 있는지 확인하기 위해 두 벡터의 외적 계산 
		FVector VectorProduct = FVector::CrossProduct(ControllingPawnForwardVector, TargetVector);

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Target To Angle %f "), TargetAngle));
		//UE_LOG(LogTemp, Log, TEXT("TargetAngle : %f, VectorProduct : %f"), TargetAngle, VectorProduct.Z);

		if (TargetAngle > -50 && TargetAngle < 50)
		{
			NotifyTurnActionEnd();
		}
		else if (VectorProduct.Z < 0)
		{
			AnimInstance->Montage_Play(LeftTurnActionMontage, AttackSpeedRate);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::TurnActionEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, LeftTurnActionMontage);
		}
		else
		{
			AnimInstance->Montage_Play(RightTurnActionMontage, AttackSpeedRate);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::TurnActionEnd);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, RightTurnActionMontage);
		}
	}
}

void ADBEnemy_ArmoredBoss::PlayJumpAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(JumpAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBEnemy_ArmoredBoss::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpAttackActionMontage);
}

void ADBEnemy_ArmoredBoss::AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ResetHitPlayers();
	NotifyComboActionEnd();
}

void ADBEnemy_ArmoredBoss::TurnActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	NotifyTurnActionEnd();
}

void ADBEnemy_ArmoredBoss::CheckTagetLocationBegin()
{
	bCheckMotionWarping = true;
}

void ADBEnemy_ArmoredBoss::CheckTagetLocationEnd()
{
	bCheckMotionWarping = false;
}

void ADBEnemy_ArmoredBoss::EnemyJumpStateBegin()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void ADBEnemy_ArmoredBoss::EnemyJumpStateEnd()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ADBEnemy_ArmoredBoss::UpdateMotionWarpingRotation(FVector MovementVector)
{
	FMotionWarpingTarget Target;

	if (MovementVector == FVector::Zero())
	{
		Target.Name = TEXT("TurnToTarget");
		
		Target.Location = GetActorForwardVector();

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
	}
	else
	{
		MovementVector.Z = 0.0f;

		Target.Name = TEXT("TurnToTarget");

		Target.Location = MovementVector;

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
	}
	
}

void ADBEnemy_ArmoredBoss::ResetMotionWarpingRotation()
{
}

#pragma region Sweep Collision

void ADBEnemy_ArmoredBoss::SwordAttackCheck()
{
	float AttackRange = 350.0f;
	float AttackRadius = 20.0f;

	FVector ColliderLocation = GetMesh()->GetSocketLocation(TEXT("Bip001-R-Hand"));

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		HitResults,
		ColliderLocation,
		ColliderLocation + WeaponMesh->GetForwardVector() * AttackRange,
		WeaponCollision->GetComponentQuat(),
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = WeaponMesh->GetForwardVector() * AttackRange;
	FVector Center = ColliderLocation + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = WeaponCollision->GetComponentQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	if (bResult)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor()->Tags.Contains(FName(TEXT("Player"))) && !HitPlayers.Contains(HitResult.GetActor()))
			{
				DB_LOG(LogDBNetwork, Log, TEXT("Find Player!"));

				HitPlayers.Emplace(HitResult.GetActor());
				AttackHitConfirm(HitResult.GetActor());
			}
		}
	}
}

void ADBEnemy_ArmoredBoss::ShieldAttackCheck()
{
	float AttackRange = 200.0f;
	float AttackRadius = 75.0f;

	FVector ColliderLocation = GetMesh()->GetSocketLocation(TEXT("Bip001-L-Hand"));
	FQuat ColliderQuat = ShieldCollision->GetComponentQuat();
	ColliderQuat = ColliderQuat.RotateVector(FVector(90.0f, 0.0f, 0.0f)).ToOrientationQuat();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		HitResults,
		ColliderLocation,
		ColliderLocation + ShieldMesh->GetForwardVector() * AttackRange,
		ColliderQuat,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = ShieldMesh->GetForwardVector() * AttackRange;
	FVector Center = ColliderLocation + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = ColliderQuat;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	if (bResult)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor()->Tags.Contains(FName(TEXT("Player"))) && !HitPlayers.Contains(HitResult.GetActor()))
			{
				DB_LOG(LogDBNetwork, Log, TEXT("Find Player!"));

				HitPlayers.Emplace(HitResult.GetActor());
				AttackHitConfirm(HitResult.GetActor());
			}
		}
	}
}

void ADBEnemy_ArmoredBoss::AroundAttackCheck()
{
	float AttackRange = 100.0f;
	float AttackRadius = 450.0f;

	FVector ColliderLocation = GetActorLocation();
	FQuat ColliderQuat = FQuat::Identity;
	ColliderQuat = ColliderQuat.RotateVector(FVector(90.0f, 0.0f, 0.0f)).ToOrientationQuat();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		HitResults,
		ColliderLocation,
		ColliderLocation,
		ColliderQuat,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector Center = ColliderLocation;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = ColliderQuat;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	if (bResult)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor()->Tags.Contains(FName(TEXT("Player"))) && !HitPlayers.Contains(HitResult.GetActor()))
			{
				DB_LOG(LogDBNetwork, Log, TEXT("Find Player!"));

				HitPlayers.Emplace(HitResult.GetActor());
				AttackHitConfirm(HitResult.GetActor());
			}
		}
	}
}

void ADBEnemy_ArmoredBoss::ForwardAttackCheck()
{
	float AttackRange = 100.0f;
	float AttackRadius = 200.0f;

	FVector ColliderLocation = WeaponCollision->GetComponentLocation() + GetActorForwardVector() * AttackRange;
	FQuat ColliderQuat = FQuat::Identity;
	ColliderQuat = ColliderQuat.RotateVector(FVector(90.0f, 0.0f, 0.0f)).ToOrientationQuat();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		HitResults,
		ColliderLocation,
		ColliderLocation,
		ColliderQuat,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG

	FVector Center = ColliderLocation;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = ColliderQuat;
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

#endif

	if (bResult)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor()->Tags.Contains(FName(TEXT("Player"))) && !HitPlayers.Contains(HitResult.GetActor()))
			{
				DB_LOG(LogDBNetwork, Log, TEXT("Find Player!"));

				HitPlayers.Emplace(HitResult.GetActor());
				AttackHitConfirm(HitResult.GetActor());
			}
		}
	}
}

#pragma endregion

void ADBEnemy_ArmoredBoss::ResetHitPlayers()
{
	HitPlayers.Empty();
}

void ADBEnemy_ArmoredBoss::AttackHitConfirm(AActor* HitActor)
{
	const float AttackDamage = Stat->GetTotalStat().Attack;
	FDamageEvent DamageEvent;
	HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
}
