// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"
#include "UI/DBHUDWidget.h"
#include "UI/DBInventoryWidget.h"
#include "UI/DBMultiUIWidget.h"
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

	static ConstructorHelpers::FClassFinder<UDBMultiUIWidget> DBMultiUIWidgetRef(TEXT("/Game/UI/WBP_DBMultiUI.WBP_DBMultiUI_C"));
	if (DBMultiUIWidgetRef.Class)
	{
		DBMultiUIWidgetClass = DBMultiUIWidgetRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Inventory.IA_Player_Inventory'");
	if (InventoryRef.Object)
	{
		InventoryAction = InventoryRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_Interaction.IA_Player_Interaction'");
	if (InteractionRef.Object)
	{
		InteractionAction = InteractionRef.Object;
	}

	NetworkSetting = CreateDefaultSubobject<ADBNetworkSetting>(TEXT("NetworkSetting"));

	bShowMouseCursor = false;
	bIsCanMultiUIWidgetOn = false;
}

//��Ʈ��ũ�� �����ϰ� ���͸� �ʱ�ȭ�� �� ���
void ADBPlayerController::PostInitializeComponents()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

//���� Ŭ���̾�Ʈ���� ��Ʈ��ũ�� �ʱ�ȭ�� �ʿ��� ������ ���޹��� ���� ��� �������� �Ǹ� ȣ��
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
			DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		DBMultiUIWidget = CreateWidget<UDBMultiUIWidget>(this, DBMultiUIWidgetClass);
		if (DBMultiUIWidget)
		{
			DBMultiUIWidget->AddToViewport();
			DBMultiUIWidget->BindingButtons(NetworkSetting);
			DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	enhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ADBPlayerController::PlayerInteractionAction);
}

void ADBPlayerController::SetMultiUIWidgetDisable()
{
	bIsCanMultiUIWidgetOn = false;
	
	//������ �Ѿ�� �ڵ����� UI�ݱ�
	if (DBMultiUIWidget)
	{
		DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
	}
}

void ADBPlayerController::PlayerInventoryAction(const FInputActionValue& Value)
{
	//MultiUI ���� ��ȣ�ۿ�
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

void ADBPlayerController::PlayerInteractionAction(const FInputActionValue& Value)
{
	if (bIsCanMultiUIWidgetOn)
	{
		DBMultiUIWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		//DBMultiUIWidget->SetFocus();
	}
	else
	{
		DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
	}
}
