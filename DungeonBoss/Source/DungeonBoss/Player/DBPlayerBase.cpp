// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "ABCharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "GameData/DAComboActionData.h"
#include "GameData/DAGuardActionData.h"
#include "DungeonBoss.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ADBPlayerBase::ADBPlayerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UABCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule 설정
	GetCapsuleComponent()->InitCapsuleSize(35.f, 88.0f);

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
	bIsAttack = false;
	HasNextCombo = false;
	bIsGuard = false;
	bIsDodge = false;

	//SkeletalMesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/PlayerCharacter.PlayerCharacter'"));
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

	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("Bip001-R-Hand"));
	Weapon->SetRelativeLocation(FVector3d(8.0f, 0.0f, 2.0f));
	Weapon->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));
}

// Called when the game starts or when spawned
void ADBPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	const float AttackSpeedRate = 1.0f;
	AttackTime = (ComboActionData->RequireComboFrame[0] / ComboActionData->FrameRate) / AttackSpeedRate;
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

void ADBPlayerBase::GuardActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsGuard = false;
}

#pragma endregion

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

#pragma region AttackCheck

void ADBPlayerBase::CheckHitAttack() 
{
	
}
void ADBPlayerBase::NextComboCheck() {}

void ADBPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADBPlayerBase, bIsAttack);
	//DOREPLIFETIME(ADBPlayerBase, HasNextCombo);
	//DOREPLIFETIME(ADBPlayerBase, bIsGuard);
	//DOREPLIFETIME(ADBPlayerBase, bIsDodge);
	//DOREPLIFETIME(ADBPlayerBase, bCanAnimationOut);
}

#pragma endregion

#pragma region Notify

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

#pragma endregion
