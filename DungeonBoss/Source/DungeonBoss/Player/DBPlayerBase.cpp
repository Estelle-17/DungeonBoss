// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "ABCharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "GameData/DAComboActionData.h"
#include "GameData/DAGuardActionData.h"
#include "GameData/DAChargeAttackActionData.h"
#include "Stat/DBCharacterStatComponent.h"
#include "DungeonBoss.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "MotionWarpingComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/DBHUDWidget.h"
#include "UI/Inventory/DBInventoryWidget.h"
#include "DBPlayerItemComponent.h"
#include "DBPlayerController.h"
#include "DBInteractionBetweenPlayerAndNPC.h"

// Sets default values
ADBPlayerBase::ADBPlayerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UABCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule 설정
	GetCapsuleComponent()->InitCapsuleSize(35.f, 88.0f);
	GetCapsuleComponent()->ComponentTags.Add(FName("Player"));
	Tags.Add(FName("Player"));
	if (HasAuthority() || IsLocallyControlled())
	{
		GetCapsuleComponent()->ComponentTags.Add(FName("LocallyControlled"));
		Tags.Add(FName("LocallyControlled"));
	}

	//Mesh 설정
	GetMesh()->SetRelativeLocation(FVector3d(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//카메라 암 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 90));
	SpringArm->TargetArmLength = 450;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bUsePawnControlRotation = true;

	//카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;

	//Attack
	CurrentCombo = 0;
	MaxCombo = 3;
	MaxChargeStack = 3;
	bIsAttack = false;
	HasNextCombo = false;
	bIsGuard = false;
	bIsDodge = false;

	//MotionWarping
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	TargetVector = FVector::Zero();

	//SkeletalMesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/Player/PlayerCharacter.PlayerCharacter'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	//AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	//ComboAction
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerAttack.AM_PlayerAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDAComboActionData> ComboActionDataRef(TEXT("/Script/DungeonBoss.DAComboActionData'/Game/GameData/DA_ComboAction.DA_ComboAction'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	//GuardAction
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GuardActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerGuard.AM_PlayerGuard'"));
	if (GuardActionMontageRef.Object)
	{
		GuardActionMontage = GuardActionMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDAGuardActionData> GuardActionDataRef(TEXT("/Script/DungeonBoss.DAGuardActionData'/Game/GameData/DA_GuardAction.DA_GuardAction'"));
	if (GuardActionDataRef.Object)
	{
		GuardActionData = GuardActionDataRef.Object;
	}

	//DodgeAction
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerDodge.AM_PlayerDodge'"));
	if (DodgeActionMontageRef.Object)
	{
		DodgeActionMontage = DodgeActionMontageRef.Object;
	}

	//ChargeAttackAction
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChargeAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerChargeAttack.AM_PlayerChargeAttack'"));
	if (ChargeAttackMontageRef.Object)
	{
		ChargeAttackActionMontage = ChargeAttackMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDAChargeAttackActionData> ChargeAttackActionDataRef(TEXT("/Script/DungeonBoss.DAChargeAttackActionData'/Game/GameData/DA_ChargeAttackAction.DA_ChargeAttackAction'"));
	if (ChargeAttackActionDataRef.Object)
	{
		ChargeAttackActionData = ChargeAttackActionDataRef.Object;
	}

	//DamageReceive Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageReceiveActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerDamageReceive.AM_PlayerDamageReceive'"));
	if (DamageReceiveActionMontageRef.Object)
	{
		DamageReceiveActionMontage = DamageReceiveActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageReceiveDownActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_PlayerDamageReceiveDown.AM_PlayerDamageReceiveDown'"));
	if (DamageReceiveDownActionMontageRef.Object)
	{
		DamageReceiveDownActionMontage = DamageReceiveDownActionMontageRef.Object;
	}

	//Stat Section
	Stat = CreateDefaultSubobject<UDBCharacterStatComponent>(TEXT("Stat"));

	//Item Section
	Inventory = CreateDefaultSubobject<UDBPlayerItemComponent>(TEXT("Inventory"));

	//Widget Section
	//HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));

	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("Bip001-R-Hand"));
	Weapon->SetRelativeLocation(FVector3d(8.0f, 0.0f, 2.0f));
	Weapon->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/Player/greatsword.greatsword'"));
	if (WeaponMeshRef.Object)
	{
		Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	//Collision Section
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Weapon);
	WeaponCollision->SetRelativeLocation(FVector3d(0.0f, 75.0f, 0.0f));
	WeaponCollision->SetRelativeScale3D(FVector3d(0.2f, 3.0f, 0.2f));
	WeaponCollision->SetGenerateOverlapEvents(true);
	WeaponCollision->SetCollisionProfileName(TEXT("WeaponPreset"));
	WeaponCollision->ComponentTags.Add(FName("Weapon"));

	SearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SearchCollision"));
	SearchCollision->SetupAttachment(RootComponent);
	SearchCollision->SetRelativeLocation(FVector3d(0.0f, 0.0f, 0.0f));
	SearchCollision->SetRelativeScale3D(FVector3d(6.5f, 6.5f, 6.5f));
	SearchCollision->SetGenerateOverlapEvents(true);
	SearchCollision->SetCollisionProfileName(TEXT("SearchCollision"));

	if (HasAuthority() || IsLocallyControlled())
	{
		DBInteractionBetweenPlayerAndNPC = CreateDefaultSubobject<UDBInteractionBetweenPlayerAndNPC>(TEXT("InteractionBetweenPlayerAndNPC"));
	}

	bIsDamageImmunity = false;
	bRecentlyDamageReceive = false;
}

// Called when the game starts or when spawned
void ADBPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	const float AttackSpeedRate = 1.0f;
	AttackTime = (ComboActionData->RequireComboFrame[0] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (!HasAuthority() || !IsLocallyControlled())
	{
		SearchCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//직접 조종하는 플레이어가 아닐 경우에는 인지하지 않음
	if (HasAuthority() || IsLocallyControlled())
	{
		DBInteractionBetweenPlayerAndNPC->PlayerSearchOverlapSetting(this);
	}

	//RootMotionMode 변경
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
}

// Called every frame
void ADBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADBPlayerBase::CheckNextAnimation(int32 CheckNumber)
{
	//1 : Attack, 2 : Dodge
	switch (CheckNumber)
	{
		case 1:
			DodgeTimerHandle.Invalidate();
			bIsDodge = false;
			bCanAnimationOut = false;

			JumpToComboAction();
			break;
		case 2:
			NextComboTimerHandle.Invalidate();
			CurrentCombo = 0;
			bIsAttack = false;
			bCanAnimationOut = false;

			MontageAnimationOut();
			ProcessGuardCommand();
			break;
	}
}

void ADBPlayerBase::MontageAnimationOut()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
}

#pragma region ComboAttack

void ADBPlayerBase::ProcessCombeStartCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
	}
}

void ADBPlayerBase::ProcessNextCombeCommand()
{
	if (!HasNextCombo)
	{
		HasNextCombo = true;
	}
}

void ADBPlayerBase::JumpToComboAction()
{
	CurrentCombo = 1;

	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	bIsAttack = true;
	NextComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ADBPlayerBase::ComboActionBegin()
{
	CurrentCombo = 1;

	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	bIsAttack = true;
	NextComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ADBPlayerBase::OnRepCheckAttack()
{

}

void ADBPlayerBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	
	//첫번째 기본 공격 시간
	const float AttackSpeedRate = 1.0f;
	AttackTime = (ComboActionData->RequireComboFrame[0] / ComboActionData->FrameRate) / AttackSpeedRate;

	bIsAttack = false;
	HasNextCombo = false;
}

void ADBPlayerBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	//DataAsset내의 ComboFrame배열에 값이 존재하는지 확인
	ensure(ComboActionData->ComboFrame.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	//다음 공격이 진행되는 시간 체크
	float NextComboEffectiveTime = (ComboActionData->RequireComboFrame[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	AttackTime = NextComboEffectiveTime;

	//클라이언트와 서버 간의 핑 차이 계산
	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, AttackTime - 0.01f);
	if (AttackTime - AttackTimeDifference > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(NextComboTimerHandle, this, &ADBPlayerBase::ComboCheck, AttackTime, false);
	}
}

void ADBPlayerBase::ComboCheck()
{
	NextComboTimerHandle.Invalidate();
	if (HasNextCombo)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->SectionPrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

		bCheckMotionWarping = true;

		SetComboCheckTimer();
		HasNextCombo = false;
	}
}

#pragma endregion

#pragma region Guard

void ADBPlayerBase::ProcessGuardCommand()
{
	if (bIsGuard)
	{
		return;
	}

	GuardActionBegin();
}

void ADBPlayerBase::GuardActionBegin()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(GuardActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::GuardActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, GuardActionMontage);

	bIsGuard = true;
	
	bIsGuardState = false;
	bCanCounterAttack = false;
}

void ADBPlayerBase::SuccessGuard(AActor* NewTarget)
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(TEXT("SuccessGuard"), GuardActionMontage);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Success Guard : %s From %s"), *GetFName().ToString(), *NewTarget->GetFName().ToString()));

	//때린 적을 바라보고 가드하도록 회전
	FMotionWarpingTarget EnemyTarget;

	EnemyTarget.Name = "TurnToTarget";
	EnemyTarget.Location = NewTarget->GetActorLocation();

	MotionWarpingComponent->AddOrUpdateWarpTarget(EnemyTarget);

	bIsGuardState = true;
	bCanCounterAttack = true;
}

void ADBPlayerBase::CounterAttackAction()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(TEXT("CounterAttack"), GuardActionMontage);

	bIsGuardState = false;
	bCanCounterAttack = false;
}

void ADBPlayerBase::GuardActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsGuard = false;
}

#pragma endregion

#pragma region Dodge

void ADBPlayerBase::ProcessDodgeCommand()
{
	if (bIsDodge)
	{
		return;
	}

	DodgeActionBegin();
}

void ADBPlayerBase::DodgeActionBegin()
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DodgeActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::DodgeActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeActionMontage);

	bIsDodge = true;

	DodgeTimerHandle.Invalidate();
	SetDodgeCheckTimer();
}

void ADBPlayerBase::SetDodgeCheckTimer()
{
	const float AttackSpeedRate = 1.0f;

	float DodgeEffectiveTime = (DodgeTime / 30.0f) / AttackSpeedRate;

	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ADBPlayerBase::DodgeTimeEnd, DodgeEffectiveTime, false);
}

void ADBPlayerBase::DodgeTimeEnd()
{
	DodgeTimerHandle.Invalidate();
}

void ADBPlayerBase::DodgeActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsDodge = false;
}

#pragma endregion

#pragma region ChargeAttack

void ADBPlayerBase::ProcessChargeAttackCommand()
{
	if (bIsChargeAttack)
	{
		return;
	}

	ChargingActionBegin();
}
//우클릭을 누르는 동안 차징 시작
void ADBPlayerBase::ChargingActionBegin()
{
	CurrentChargeStack = 0;

	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ChargeAttackActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::ChargeAttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ChargeAttackActionMontage);

	bIsChargeAttack = true;
	ChargeTimerHandle.Invalidate();
	SetChargeCheckTimer();
}
//차징된 양을 기준으로 공격 애니메이션 출력
void ADBPlayerBase::ChargeAttackBegin()
{
	if (CurrentChargeStack == 0)
	{
		/*MontageAnimationOut();
		ChargeTimerHandle.Invalidate();
		bIsChargeAttack = false;*/
		return;
	}

	bIsFullCharge = false;
	bIsCharging = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	CurrentChargeStack = FMath::Clamp(CurrentChargeStack, 1, ChargeAttackActionData->MaxChargeStack);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *ChargeAttackActionData->SectionPrefix, CurrentChargeStack);
	AnimInstance->Montage_JumpToSection(NextSection, ChargeAttackActionMontage);

	CurrentChargeStack = -1;
	ChargeTimerHandle.Invalidate();
}

void ADBPlayerBase::SetChargeCheckTimer()
{
	ensure(ChargeAttackActionData->RequireChargeFrame.IsValidIndex(CurrentChargeStack));

	const float AttackSpeedRate = 1.0f;

	//차징이 완료되는 시간 체크
	float NextComboEffectiveTime = (ChargeAttackActionData->RequireChargeFrame[CurrentChargeStack] / ChargeAttackActionData->FrameRate) / AttackSpeedRate;
	ChargingTime = NextComboEffectiveTime;

	//클라이언트와 서버 간의 핑 차이 계산
	ChargeAttackTimeDifference = FMath::Clamp(ChargeAttackTimeDifference, 0.0f, ChargingTime - 0.01f);
	if (ChargingTime - ChargeAttackTimeDifference > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &ADBPlayerBase::ChargeCheck, ChargingTime, false);
	}
}

void ADBPlayerBase::ChargeCheck()
{
	//처음 1차징동안 입력이 끝났을 경우
	if (CurrentChargeStack == 0 && !bIsCharging)
	{
		CurrentChargeStack = FMath::Clamp(CurrentChargeStack + 1, 1, ChargeAttackActionData->MaxChargeStack);
		DB_LOG(LogDBNetwork, Log, TEXT("CurrnetChargeStack : %d"), CurrentChargeStack);
		ChargeTimerHandle.Invalidate();
		ChargeAttackBegin();
		return;
	}

	if (bIsCharging)
	{
		if (bIsFullCharge)
		{
			ChargeAttackBegin();
			return;
		}

		CurrentChargeStack = FMath::Clamp(CurrentChargeStack + 1, 1, ChargeAttackActionData->MaxChargeStack);
		DB_LOG(LogDBNetwork, Log, TEXT("CurrnetChargeStack : %d"), CurrentChargeStack);
		ChargeTimerHandle.Invalidate();
		if (CurrentChargeStack <= 3)
		{
			if (CurrentChargeStack == ChargeAttackActionData->MaxChargeStack)
			{
					bIsFullCharge = true;
			}
			SetChargeCheckTimer();
		}
	}
}

void ADBPlayerBase::ChargeAttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ChargeTimerHandle.Invalidate();

	//차징 공격 중 풀고 누르면 다시 공격되니 수정 요함
	CurrentChargeStack = 0;
	bIsChargeAttack = false;
}

#pragma endregion

#pragma region HitAction

void ADBPlayerBase::ProcessDamageReceiveCommand(AActor* NewTarget)
{
	MontageAnimationOut();

	if (bRecentlyDamageReceive)
	{
		DamageReceiveDownActionBegin(NewTarget);
	}
	else
	{
		DamageReceiveActionBegin(NewTarget);
	}
}

void ADBPlayerBase::DamageReceiveActionBegin(AActor* NewTarget)
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DamageReceiveActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::DamageReceiveActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DamageReceiveActionMontage);

	//때린 적을 바라보고 가드하도록 회전
	FMotionWarpingTarget EnemyTarget;

	EnemyTarget.Name = "TurnToTarget";
	EnemyTarget.Location = NewTarget->GetActorLocation();

	MotionWarpingComponent->AddOrUpdateWarpTarget(EnemyTarget);

	bRecentlyDamageReceive = true;
	bIsPlayingDamageReceiveAction = true;

	DamageReceiveTimerHandle.Invalidate();
	SetDamageReceiveCheckTimer();
}

void ADBPlayerBase::DamageReceiveDownActionBegin(AActor* NewTarget)
{
	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DamageReceiveDownActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADBPlayerBase::DamageReceiveActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DamageReceiveDownActionMontage);

	//때린 적을 바라보고 가드하도록 회전
	FMotionWarpingTarget EnemyTarget;

	EnemyTarget.Name = "TurnToTarget";
	EnemyTarget.Location = NewTarget->GetActorLocation();

	MotionWarpingComponent->AddOrUpdateWarpTarget(EnemyTarget);

	bRecentlyDamageReceive = false;
	bIsPlayingDamageReceiveAction = true;

	DamageReceiveTimerHandle.Invalidate();
}

void ADBPlayerBase::SetDamageReceiveCheckTimer()
{
	const float AttackSpeedRate = 1.0f;

	GetWorld()->GetTimerManager().SetTimer(DamageReceiveTimerHandle, this, &ADBPlayerBase::DamageReceiveTimeEnd, RecentlyDamageReceiveCheckTime, false);
}

void ADBPlayerBase::DamageReceiveTimeEnd()
{
	bRecentlyDamageReceive = false;

	DamageReceiveTimerHandle.Invalidate();
}

void ADBPlayerBase::DamageReceiveActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsPlayingDamageReceiveAction = false;
}

#pragma endregion

#pragma region MotionWarping

void ADBPlayerBase::SetMotionWarpingRotation(FVector MovementVector)
{
	TargetVector = MovementVector;

	FMotionWarpingTarget Target;

	if (MovementVector == FVector3d(0, 0, 0))
	{
		Target.Name = "AttackTarget";
		Target.Rotation = GetActorForwardVector().Rotation();

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
	}
	else
	{
		MovementVector.Z = 0.0f;

		Target.Name = "AttackTarget";
		Target.Rotation = MovementVector.Rotation();

		MotionWarpingComponent->AddOrUpdateWarpTarget(Target);
	}
}

void ADBPlayerBase::ResetMotionWarpingRotation()
{
	TargetVector = GetCharacterMovement()->GetLastInputVector();

	SetMotionWarpingRotation(TargetVector);
}

#pragma endregion

#pragma region AttackCheck

void ADBPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADBPlayerBase, bIsAttack);
	//DOREPLIFETIME(ADBPlayerBase, HasNextCombo);
	//DOREPLIFETIME(ADBPlayerBase, bIsGuard);
	//DOREPLIFETIME(ADBPlayerBase, bIsDodge);
	//DOREPLIFETIME(ADBPlayerBase, bCanAnimationOut);
}

float ADBPlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsDamageImmunity)
	{
		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Blue, FString::Printf(TEXT("DamageImmunity Time")));
		return DamageAmount;
	}

	if (bIsGuardState)
	{
		SuccessGuard(DamageCauser);
	}
	else
	{
		Stat->ApplyDamage(DamageAmount);
		ProcessDamageReceiveCommand(DamageCauser);
	}

	return DamageAmount;
}

#pragma endregion

#pragma region UI

void ADBPlayerBase::SetupHUDWidget(UDBHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UDBHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UDBHUDWidget::UpdateHpBar);

		OnUpdateEnemyHpBar.AddUObject(InHUDWidget, &UDBHUDWidget::UpdateBossHpBar);
		EnemyHpBar = InHUDWidget->GetEnemyHpBarWidget();
		OnUpdatePlayerDescription.AddUObject(InHUDWidget, &UDBHUDWidget::UpdatePlayerDescription);
		OnDisablePlayerDescription.AddUObject(InHUDWidget, &UDBHUDWidget::DisablePlayerDescription);

		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Blue, FString::Printf(TEXT("InHUBWidget Setting Complete!")));
	}
}

void ADBPlayerBase::SetupInventoryWidget(UDBInventoryWidget* InInventoryWidget)
{
	if (InInventoryWidget)
	{
		Inventory->OnAddEquipItem.AddUObject(InInventoryWidget, &UDBInventoryWidget::AddEquipItem);
		Inventory->OnAddCountableItem.AddUObject(InInventoryWidget, &UDBInventoryWidget::AddCountableItem);
	}
}

//void ADBPlayerBase::OnSearchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor->Tags.Contains(FName(TEXT("MultiplayerNPC"))))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());
//		//특정 범위 내에 플레이어가 있을 경우 MultiUI를 열 수 있는 상태인 것을 알려줌
//
//		ADBPlayerController* DBPlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());
//
//		if (DBPlayerController)
//		{
//			UE_LOG(LogTemp, Log, TEXT("SetMultiUIWidgetEnable"));
//			DBPlayerController->SetMultiUIWidgetEnable();
//		}
//	}
//	else if (OtherActor->Tags.Contains(FName(TEXT("ItemMakingNPC"))))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());
//		//특정 범위 내에 플레이어가 있을 경우 ItemMakingNPCUI를 열 수 있는 상태인 것을 알려줌
//
//		
//	}
//}
//
//void ADBPlayerBase::OnSearchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor->Tags.Contains(FName(TEXT("MultiplayerNPC"))))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());
//		//특정 범위 내에 플레이어가 있을 경우 MultiUI를 열 수 있는 상태인 것을 알려줌
//
//		ADBPlayerController* DBPlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());
//
//		if (DBPlayerController)
//		{
//			UE_LOG(LogTemp, Log, TEXT("SetMultiUIWidgetDisable"));
//			DBPlayerController->SetMultiUIWidgetDisable();
//		}
//	}
//	else if (OtherActor->Tags.Contains(FName(TEXT("ItemMakingNPC"))))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());
//		//특정 범위 내에 플레이어가 있을 경우 MultiUI를 열 수 있는 상태인 것을 알려줌
//
//		
//	}
//}

#pragma endregion

#pragma region Notify

void ADBPlayerBase::CheckHitEnable()
{
	//UE_LOG(LogTemp, Log, TEXT("CheckHitEnable!"));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADBPlayerBase::CheckHitDisable()
{
	//UE_LOG(LogTemp, Log, TEXT("CheckHitDisable!"));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitEnemies.Empty();
}

void ADBPlayerBase::CheckEnableComboTime()
{
	if (bIsAttack)
	{
		if (!NextComboTimerHandle.IsValid() && HasNextCombo)
		{
			ComboCheck();
		}
	}
}

void ADBPlayerBase::EnableGuardTime()
{
	if (bIsGuard)
	{
		bIsGuardState = true;
	}
}

void ADBPlayerBase::DisableGuardTime()
{
	if (bIsGuard)
	{
		bIsGuardState = false;
	}
}

void ADBPlayerBase::AnimationOutEnable()
{
	bCanAnimationOut = true;
}

void ADBPlayerBase::AnimationOutDisable()
{
	bCanAnimationOut = false;
}

void ADBPlayerBase::DamageImmunityEnable()
{
	bIsDamageImmunity = true;
}

void ADBPlayerBase::DamageImmunityDisable()
{
	bIsDamageImmunity = false;
}

void ADBPlayerBase::UpdateMotionWarpingTargetVector()
{
	bCheckMotionWarping = true;
	//UE_LOG(LogTemp, Log, TEXT("UpdateMotionWarpingTargetVector On"));
}

#pragma endregion
