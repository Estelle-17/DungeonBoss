// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"
#include "UI/DBHUDWidget.h"
#include "UI/DBInventoryWidget.h"
#include "EnhancedInputComponent.h"

ADBPlayerController::ADBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UDBHUDWidget> DBHUDWidgetRef(TEXT("/Game/UI/WBP_DBHUD.WBP_DBHUD_C"));
	if (DBHUDWidgetRef.Class)
	{
		DBHUDWidgetClass = DBHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDBInventoryWidget> DBInventoryWidgetRef(TEXT("/Game/UI/Item/WBP_Inventory.WBP_Inventory_C"));
	if (DBInventoryWidgetRef.Class)
	{
		DBInventoryWidgetClass = DBInventoryWidgetRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Inventory.IA_Player_Inventory'");
	if (InventoryRef.Object)
	{
		InventoryAction = InventoryRef.Object;
	}
}

//네트워크와 무관하게 액터를 초기화할 때 사용
void ADBPlayerController::PostInitializeComponents()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

//원격 클라이언트에서 네트워크로 초기화에 필요한 정보를 전달받은 것이 모두 마무리가 되면 호출
void ADBPlayerController::PostNetInit()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();

	if (NetDriver)
	{
		if (NetDriver->ServerConnection)
		{
			DB_LOG(LogDBNetwork, Log, TEXT("Server Connections : %s"), *NetDriver->ServerConnection->GetName());
		}
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (IsLocalPlayerController())
	{
		DBHUDWidget = CreateWidget<UDBHUDWidget>(this, DBHUDWidgetClass);
		if (DBHUDWidget)
		{
			DBHUDWidget->AddToViewport();
		}

		DBInventoryWidget = CreateWidget<UDBInventoryWidget>(this, DBInventoryWidgetClass);
		if (DBInventoryWidget)
		{
			DBInventoryWidget->AddToViewport();
		}
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayerController::OnPossess(APawn* InPawn)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	enhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ADBPlayerController::PlayerInventoryAction);
}

void ADBPlayerController::PlayerInventoryAction(const FInputActionValue& Value)
{
	//DBInventoryWidget->AddEquipItem(TEXT("HEAD_001"));

	if (DBInventoryWidget->IsVisible())
	{
		DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;		
	}
	else
	{
		DBInventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
	}
}
