// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ABCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "DungeonBoss.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//EnhancedInputAction
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef = TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'");
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterMoveRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Move.IA_Player_Move'");
	if (QuaterMoveRef.Object)
	{
		MoveAction = QuaterMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterLookRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Look.IA_Player_Look'");
	if (QuaterLookRef.Object)
	{
		LookAction = QuaterLookRef.Object;
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

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerMove);
	enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayerLook);
}

void APlayerCharacter::PlayerMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVectorSizeSquared = 1.0f;
		MovementVector.Normalize();
	}

	FVector ForwardDirection = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APlayerCharacter::PlayerLook(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}