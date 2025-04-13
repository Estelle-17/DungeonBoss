// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ABCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/DamageEvents.h"
#include "EngineUtils.h"
#include "DungeonBoss.h"
#include "MotionWarpingComponent.h"
#include "Stat/DBCharacterStatComponent.h"
#include "DBPlayerItemComponent.h"
#include "DBInteractionBetweenPlayerAndNPC.h"
#include "Stat/DBEnemyStatComponent.h"
#include "GM/DBStageMoveActor.h"
#include "UI/DBEnemyHpBarWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.033f;

	//EnhancedInputAction
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef = TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'");
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerMoveRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Move.IA_Player_Move'");
	if (PlayerMoveRef.Object)
	{
		MoveAction = PlayerMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerLookRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Look.IA_Player_Look'");
	if (PlayerLookRef.Object)
	{
		LookAction = PlayerLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerAttackRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Attack.IA_Player_Attack'");
	if (PlayerAttackRef.Object)
	{
		AttackAction = PlayerAttackRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerChargeAttackRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_ChargeAttack.IA_Player_ChargeAttack'");
	if (PlayerChargeAttackRef.Object)
	{
		ChargeAttackAction = PlayerChargeAttackRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerGuardOrDodgeRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Guard_Dodge.IA_Player_Guard_Dodge'");
	if (PlayerGuardOrDodgeRef.Object)
	{
		GuardOrDodgeAction = PlayerGuardOrDodgeRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Interaction.IA_Player_Interaction'");
	if (InteractionRef.Object)
	{
		InteractionAction = InteractionRef.Object;
	}

	//Collision Section
	if (HasAuthority() || IsLocallyControlled())
	{
		WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
		WeaponCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"))

	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"))
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	Super::PossessedBy(NewController);

	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCheckMotionWarping)
	{
		if (HasAuthority())
		{
			SetMotionWarpingRotation(GetCharacterMovement()->GetLastInputVector());
			UE_LOG(LogTemp, Log, TEXT("Check MotionWarping Vector : %s"), *GetCharacterMovement()->GetLastInputVector().ToString());
		}
		else if (IsLocallyControlled())
		{
			ServerRPCUpdateTargetVector(GetCharacterMovement()->GetLastInputVector());
			UE_LOG(LogTemp, Log, TEXT("Check MotionWarping Vector : %s"), *GetCharacterMovement()->GetLastInputVector().ToString());
		}

		bCheckMotionWarping = false;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerMove);
	enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerLook);
	enhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerAttack);
	enhancedInputComponent->BindAction(ChargeAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::PlayerChargeAttackEnable);
	enhancedInputComponent->BindAction(ChargeAttackAction, ETriggerEvent::Canceled, this, &APlayerCharacter::PlayerChargeAttackDisable);
	enhancedInputComponent->BindAction(ChargeAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::PlayerChargeAttackDisable);
	enhancedInputComponent->BindAction(GuardOrDodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerGuardOrDodge);
	enhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerInteractionAction);
}

void APlayerCharacter::PlayerMove(const FInputActionValue& Value)
{
	if (bCanAnimationOut)
	{
		CheckNextAnimation(0);
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVectorSizeSquared = 1.0f;
		MovementVector.Normalize();
	}

	FRotator ControlRotation(0, GetControlRotation().Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APlayerCharacter::PlayerLook(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::PlayComboAttack()
{
	DB_LOG(LogDBNetwork, Log, TEXT("bCanAnimationOut : %s"), bCanAnimationOut ? TEXT("True") : TEXT("false"));
	if (bCanAnimationOut)
	{
		CheckNextAnimation(1);
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("CurrentCombo : %d"), CurrentCombo);
		if (CurrentCombo != 0)
		{
			ProcessNextCombeCommand();
		}
		else
		{
			ProcessCombeStartCommand();
		}
	}
}

void APlayerCharacter::PlayGuard()
{
	if (bCanAnimationOut)
	{
		CheckNextAnimation(2);
	}
	else if (!bIsGuard)
	{
		ProcessGuardCommand();
	}
}

void APlayerCharacter::PlayDodge()
{
	if (bCanAnimationOut)
	{
		CheckNextAnimation(3);
	}
	else if (!bIsDodge)
	{
		ProcessDodgeCommand();
	}
}

void APlayerCharacter::StartCharge()
{
	if (bCanAnimationOut)
	{
		CheckNextAnimation(4);
	}
	else if (!bIsDodge)
	{
		bIsCharging = true;
		ProcessChargeAttackCommand();
	}
}

void APlayerCharacter::EndCharge()
{
	ChargeAttackBegin();
	bIsCharging = false;
}

void APlayerCharacter::PlayerAttack(const FInputActionValue& Value)
{
	if (bIsGuard && bCanCounterAttack && !bIsDodge && !bIsChargeAttack && !bIsPlayingDamageReceiveAction)
	{
		if (!HasAuthority())
		{
			CounterAttackAction();
		}
		ServerRPCCounterAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}

	if (!bIsGuard && !bIsDodge && !bIsChargeAttack && !bIsPlayingDamageReceiveAction || bCanAnimationOut)
	{
		if (!HasAuthority())
		{
			PlayComboAttack();
		}
		//Inventory->UpdateEquipItem(TEXT("WEAPON_001"));
		//Inventory->UpdateCountableItem(TEXT("BOSS_01_01"), 1);
		ServerRPCAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void APlayerCharacter::PlayerChargeAttackEnable(const FInputActionValue& Value)
{
	DB_LOG(LogDBNetwork, Log, TEXT("PlayerChargeStart"));
	if (!bIsCharging && !bIsChargeAttack && !bIsAttack && !bIsGuard && !bIsDodge && !bIsPlayingDamageReceiveAction || bCanAnimationOut)
	{
		if (!HasAuthority())
		{
			StartCharge();
		}
		ServerRPCChargeAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}

	if (bIsChargeAttack && CurrentChargeStack == 0)
	{
		if (!HasAuthority())
		{
			StartCharge();
		}
		ServerRPCChargeAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void APlayerCharacter::PlayerChargeAttackDisable(const FInputActionValue& Value)
{
	DB_LOG(LogDBNetwork, Log, TEXT("PlayerChargeEnd"));
	if (bIsCharging)
	{
		if (!HasAuthority())
		{
			EndCharge();
		}
		ServerRPCEndCharge();
	}
}

void APlayerCharacter::PlayerGuardOrDodge(const FInputActionValue& Value)
{
	if (bIsAttack && bIsChargeAttack && bIsPlayingDamageReceiveAction)
	{
		return;
	}

	if (GetCharacterMovement()->GetLastInputVector() == FVector3d(0, 0, 0))
	{
		if (!HasAuthority())
		{
			PlayGuard();
		}

		ServerRPCGuard(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
	else if (!bIsGuard && !bIsDodge)	//Dodge Section
	{
		if (!HasAuthority())
		{
			PlayDodge();
		}

		ServerRPCDodge(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void APlayerCharacter::PlayerInteractionAction(const FInputActionValue& Value)
{
	if (DBStageMoveActor)
	{
		DBStageMoveActor->MoveFieldWidgetOn();
		return;
	}

	DBInteractionBetweenPlayerAndNPC->InteractionNPC();
}

void APlayerCharacter::AttackHitCheck()
{
	/*if (IsLocallyControlled())
	{
		FHitResult OutHitResult;

		const float AttackRange = 1.0f;

		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool HitDetected = false;

		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		if (!HasAuthority())
		{
			if (HitDetected)
			{
				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
			}
			else
			{
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}
		else
		{
			if (HitDetected)
			{
				AttackHitConfirm(OutHitResult.GetActor());
			}
		}
	}*/
}

void APlayerCharacter::AttackHitConfirm(AActor* HitActor)
{
	if (HasAuthority())
	{
		const float AttackDamage = Stat->GetTotalStat().Attack;

		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("Hit Damage : %d"), AttackDamage));

		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
}

void APlayerCharacter::UpdateEnemyHpBar(AActor* EnemyActor)
{
	//적Hp 체크
	UDBEnemyStatComponent* EnemyStat = Cast<UDBEnemyStatComponent>(EnemyActor->GetComponentByClass(UDBEnemyStatComponent::StaticClass()));
	if (EnemyStat)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Check Enemy Hp : %f, %f"), EnemyStat->GetCurrentHp(), EnemyStat->GetTotalStat().MaxHp));

		if (EnemyHpBar)
		{
			EnemyHpBar->SetVisibility(ESlateVisibility::Visible);
			EnemyHpBar->SetMaxHp(EnemyStat->GetTotalStat().MaxHp);
		}

		//몬스터를 한번 때릴 때 한번 등록하고 이후 연속해서 등록하지 않도록 설정해야함
		if (EnemyStat->RegisteredObjects.Contains(this))
		{
			EnemyStat->OnHpChanged.AddUObject(EnemyHpBar, &UDBEnemyHpBarWidget::UpdateHpBar);
			EnemyStat->RegisteredObjects.Add(this);
		}

		//적 이름 설정
		OnUpdateEnemyHpBar.Broadcast(EnemyStat->GetCurrentHp() / EnemyStat->GetTotalStat().MaxHp, EnemyActor->GetFName());
	}
}

#pragma region Replicated

bool APlayerCharacter::ServerRPCUpdateTargetVector_Validate(FVector MoveVector)
{
	return true;
}

bool APlayerCharacter::ServerRPCAttack_Validate(float AttackStartTime)
{
	if (LastAttackStartTime == 0.0f)
	{
		return true;
	}

	return true;
	//return (AttackStartTime - LastAttackStartTime) > AttackTime;
}
bool APlayerCharacter::ServerRPCGuard_Validate(float GuardStartTime)
{
	return true;
}

bool APlayerCharacter::ServerRPCCounterAttack_Validate(float AttackStartTime)
{
	return true;
}

bool APlayerCharacter::ServerRPCDodge_Validate(float DodgeStartTime)
{
	return true;
}

bool APlayerCharacter::ServerRPCChargeAttack_Validate(float AttackStartTime)
{
	if (LastChargeAttackStartTime == 0.0f)
	{
		return true;
	}

	return true;
	//return (AttackStartTime - LastAttackStartTime) > AttackTime;
}

bool APlayerCharacter::ServerRPCEndCharge_Validate()
{
	return true;
}

bool APlayerCharacter::ServerRPCNotifyHit_Validate(AActor* OtherActor, FVector HitLocation)
{
	return true;
}

bool APlayerCharacter::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return true;
}

void APlayerCharacter::ServerRPCUpdateTargetVector_Implementation(FVector MoveVector)
{
	SetMotionWarpingRotation(MoveVector);

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->TargetVector = MoveVector;
					OtherPlayer->ClientRPCUpdateTargetVector(this, MoveVector);
				}
			}
		}
	}
}

void APlayerCharacter::ServerRPCAttack_Implementation(float AttackStartTime)
{
	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
	DB_LOG(LogDBNetwork, Log, TEXT("LagTime : %f"), AttackTimeDifference);

	PlayComboAttack();

	LastAttackStartTime = AttackStartTime;

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCProcessComboAttack(this);
				}
			}
		}
	}
}

void APlayerCharacter::ServerRPCGuard_Implementation(float GuardStartTime)
{
	GuardTimeDifference = GetWorld()->GetTimeSeconds() - GuardStartTime;
	DB_LOG(LogDBNetwork, Log, TEXT("LagTime : %f"), GuardTimeDifference);

	PlayGuard();

	LastGuardStartTime = GuardStartTime;

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCProcessGuard(this);
				}
			}
		}
	}
}

void APlayerCharacter::ServerRPCCounterAttack_Implementation(float AttackStartTime)
{
	GuardAttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
	DB_LOG(LogDBNetwork, Log, TEXT("LagTime : %f"), GuardAttackTimeDifference);

	CounterAttackAction();

	LastCounterAttackStartTime = AttackStartTime;

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCProcessCounterAttack(this);
				}
			}
		}
	}
}

void APlayerCharacter::ServerRPCDodge_Implementation(float DodgeStartTime)
{
	DodgeTimeDifference = GetWorld()->GetTimeSeconds() - DodgeStartTime;
	DB_LOG(LogDBNetwork, Log, TEXT("LagTime : %f"), DodgeTimeDifference);

	PlayDodge();

	LastDodgeStartTime = DodgeStartTime;

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCProcessDodge(this);
				}
			}
		}
	}
}

void APlayerCharacter::ServerRPCChargeAttack_Implementation(float AttackStartTime)
{
	ChargeAttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;
	DB_LOG(LogDBNetwork, Log, TEXT("LagTime : %f"), ChargeAttackTimeDifference);

	StartCharge();

	LastChargeAttackStartTime = AttackStartTime;

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCProcessChargeAttack(this);
				}
			}
		}
	}
}


void APlayerCharacter::ServerRPCEndCharge_Implementation()
{
	EndCharge();

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCEndCharge(this);
				}
			}
		}
	}
}

void APlayerCharacter::ClientRPCUpdateTargetVector_Implementation(APlayerCharacter* CharacterToPlay, FVector MoveVector)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->SetMotionWarpingRotation(MoveVector);
	}
}

void APlayerCharacter::ClientRPCProcessComboAttack_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayComboAttack();
	}
}

void APlayerCharacter::ClientRPCProcessGuard_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayGuard();
	}
}


void APlayerCharacter::ClientRPCProcessCounterAttack_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->CounterAttackAction();
	}
}

void APlayerCharacter::ClientRPCProcessDodge_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{	
		CharacterToPlay->PlayDodge();
	}
}

void APlayerCharacter::ClientRPCProcessChargeAttack_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->StartCharge();
	}
}

void APlayerCharacter::ClientRPCEndCharge_Implementation(APlayerCharacter* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->EndCharge();
	}
}

void APlayerCharacter::ClientRPCUpdateEnemyHpBar_Implementation(APlayerCharacter* CharacterToPlay, AActor* EnemyActor)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->UpdateEnemyHpBar(EnemyActor);
	}
}

void APlayerCharacter::MulticastRPCAttack_Implementation()
{

}
void APlayerCharacter::MulticastRPCGuard_Implementation()
{
	
}

void APlayerCharacter::MulticastRPCDodge_Implementation()
{
	
}

void APlayerCharacter::MulticastRPCChargeAttack_Implementation()
{
}

void APlayerCharacter::ServerRPCNotifyHit_Implementation(AActor* OtherActor, FVector HitLocation)
{
	AActor* HitActor = OtherActor;

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), *HitActor->GetName());

	if (::IsValid(HitActor))
	{
		const FVector HitActorLocation = HitActor->GetActorLocation();

/*		UE_LOG(LogTemp, Log, TEXT("Check Hit Vector : %s * %s"), *HitLocation.ToString(), *HitActorLocation.ToString());

		UE_LOG(LogTemp, Log, TEXT("Check Vector : %f <= %f"), FVector::DistSquared(HitLocation, HitActorLocation), AcceptCheckDistance * AcceptCheckDistance);*/

		if (FVector::DistSquared(HitLocation, HitActorLocation) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			AttackHitConfirm(HitActor);

			for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
			{
				if (PlayerController && GetController() != PlayerController)
				{
					if (!PlayerController->IsLocalController())
					{
						APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());

						if (OtherPlayer)
						{
							OtherPlayer->ClientRPCUpdateEnemyHpBar(this, OtherActor);
						}
					}
				}
			}
			UpdateEnemyHpBar(OtherActor);
		}
		else
		{
			DB_LOG(LogDBNetwork, Warning, TEXT("%s"), TEXT("HitTest Reject!"));
		}
	}
}
void APlayerCharacter::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{

}

#pragma endregion

#pragma region Collision

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains(FName(TEXT("Enemy"))) && !HitEnemies.Contains(OtherActor) && IsLocallyControlled())
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Find Enemy!"));

		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		if (IsLocallyControlled())
		{
			UpdateEnemyHpBar(OtherActor);
		}

		if (!HasAuthority())
		{
			HitEnemies.Emplace(OtherActor);
			ServerRPCNotifyHit(OtherActor, GetActorLocation());
		}
		else
		{
			HitEnemies.Emplace(OtherActor);
			AttackHitConfirm(OtherActor);
		}
	}
}

#pragma endregion