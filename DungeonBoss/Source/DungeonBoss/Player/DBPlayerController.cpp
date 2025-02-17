// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"
#include "UI/DBHUDWidget.h"
#include "UI/DBInventoryWidget.h"
#include "UI/DBMultiUIWidget.h"
#include "UI/DBItemDragVisualWidget.h"
#include "UI/DBItemCountScrollBarWidget.h"
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

	static ConstructorHelpers::FClassFinder<UDBItemDragVisualWidget> DBItemDragVisualWidgetRef(TEXT("/Game/UI/Item/WBP_ItemDragVisual.WBP_ItemDragVisual_C"));
	if (DBItemDragVisualWidgetRef.Class)
	{
		DBItemDragVisualWidgetClass = DBItemDragVisualWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDBMultiUIWidget> DBMultiUIWidgetRef(TEXT("/Game/UI/WBP_DBMultiUI.WBP_DBMultiUI_C"));
	if (DBMultiUIWidgetRef.Class)
	{
		DBMultiUIWidgetClass = DBMultiUIWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDBItemCountScrollBarWidget> DBItemCountScrollBarWidgetRef(TEXT("/Game/UI/Item/WBP_ItemCountScrollBar.WBP_ItemCountScrollBar_C"));
	if (DBItemCountScrollBarWidgetRef.Class)
	{
		DBItemCountScrollBarWidgetClass = DBItemCountScrollBarWidgetRef.Class;
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

	static ConstructorHelpers::FObjectFinder<UInputAction> DivideItemRef = TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Player_DivideItem.IA_Player_DivideItem'");
	if (DivideItemRef.Object)
	{
		DivideItemAction = DivideItemRef.Object;
	}

	NetworkSetting = CreateDefaultSubobject<ADBNetworkSetting>(TEXT("NetworkSetting"));

	bShowMouseCursor = false;
	bIsCanMultiUIWidgetOn = false;
	bIsCtrlClicked = false;
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
		
		DBItemDragVisualWidget = CreateWidget<UDBItemDragVisualWidget>(this, DBItemDragVisualWidgetClass);
		DBInventoryWidget = CreateWidget<UDBInventoryWidget>(this, DBInventoryWidgetClass);
		if (DBInventoryWidget)
		{
			DBInventoryWidget->AddToViewport();
			DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		//DBItemDragVisualWidget생성은 먼저하되 AddToViewport를 늦게 진행하여 UI가 앞으로 오도록 해줌
		if (DBItemDragVisualWidget)
		{
			DBItemDragVisualWidget->AddToViewport();
			DBItemDragVisualWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	enhancedInputComponent->BindAction(DivideItemAction, ETriggerEvent::Started, this, &ADBPlayerController::PlayerDivideItemActionBegin);
	enhancedInputComponent->BindAction(DivideItemAction, ETriggerEvent::Canceled, this, &ADBPlayerController::PlayerDivideItemActionEnd);
}

void ADBPlayerController::SetMultiUIWidgetDisable()
{
	bIsCanMultiUIWidgetOn = false;
	
	//범위를 넘어서면 자동으로 UI닫기
	if (DBMultiUIWidget)
	{
		DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
	}
}

void ADBPlayerController::PlayerInventoryAction(const FInputActionValue& Value)
{
	//MultiUI 위젯 상호작용
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

void ADBPlayerController::PlayerDivideItemActionBegin(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Ctrl On"));

	bIsCtrlClicked = true;
}

void ADBPlayerController::PlayerDivideItemActionEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Ctrl Off"));

	bIsCtrlClicked = false;
}


