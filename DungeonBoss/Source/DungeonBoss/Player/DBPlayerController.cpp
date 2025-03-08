// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "DungeonBoss.h"
#include "UI/DBHUDWidget.h"
#include "UI/Inventory/DBInventoryWidget.h"
#include "UI/DBMultiUIWidget.h"
#include "UI/Inventory/DBItemDragVisualWidget.h"
#include "UI/Inventory/DBItemCountScrollBarWidget.h"
#include "UI/ProductionNPC/DBProductionNPCWidget.h"
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

	bIsCanMultiUIWidgetOn = false;
	bIsRecentlyWidgetCollapsed = false;
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

void ADBPlayerController::SetNextTickEnableCanWidgetCollapsed()
{
	//이미 현 프레임에 Widget을 조작했을 경우
	//다음 틱에 Widget입력이 가능해지도록 도와줌
	GetWorld()->GetTimerManager().SetTimerForNextTick([&]
		{
			bIsRecentlyWidgetCollapsed = false;
		});
}

void ADBPlayerController::LoadWidget(UUserWidget* UIWidget)
{
	bIsRecentlyWidgetCollapsed = true;
	SetNextTickEnableCanWidgetCollapsed();

	FInputModeUIOnly InputModeData;
	//사용할 위젯 지정
	InputModeData.SetWidgetToFocus(UIWidget->TakeWidget());
	//마우스를 Viewport에 고정할건지, 자유롭게 하던지 지정
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

	//PlayerController에 적용
	SetInputMode(InputModeData);
	//마우스커서 On/Off
	bShowMouseCursor = true;
}

void ADBPlayerController::CollapseWidget(FString WidgetName)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Widget"));

	if (WidgetName.Equals(TEXT("Inventory")))
	{
		DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//모든 UI위젯이 종료되었을 때
	if (!CheckWidgetVisible())
	{
		bIsRecentlyWidgetCollapsed = true;
		SetNextTickEnableCanWidgetCollapsed();

		FInputModeGameOnly InputModeData;

		//PlayerController에 적용
		SetInputMode(InputModeData);
		//마우스커서 On/Off
		bShowMouseCursor = false;
	}
}

bool ADBPlayerController::CheckWidgetVisible()
{
	if (DBInventoryWidget->IsVisible())
	{
		return true;
	}

	return false;
}

void ADBPlayerController::PlayerInventoryAction(const FInputActionValue& Value)
{
	//MultiUI 위젯 상호작용
	if (!bIsRecentlyWidgetCollapsed)
	{
		if (DBInventoryWidget->IsVisible())
		{
			DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			CollapseWidget(TEXT("Inventory"));
		}
		else
		{
			DBInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			LoadWidget(CastChecked<UUserWidget>(DBInventoryWidget));
		}
	}
}



