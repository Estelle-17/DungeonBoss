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

		DBItemDragVisualWidget = CreateWidget<UDBItemDragVisualWidget>(this, DBItemDragVisualWidgetClass);
		DBInventoryWidget = CreateWidget<UDBInventoryWidget>(this, DBInventoryWidgetClass);
		if (DBInventoryWidget)
		{
			DBInventoryWidget->AddToViewport();
			DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		//DBItemDragVisualWidget������ �����ϵ� AddToViewport�� �ʰ� �����Ͽ� UI�� ������ ������ ����
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
	//�̹� �� �����ӿ� Widget�� �������� ���
	//���� ƽ�� Widget�Է��� ������������ ������
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
	//����� ���� ����
	InputModeData.SetWidgetToFocus(UIWidget->TakeWidget());
	//���콺�� Viewport�� �����Ұ���, �����Ӱ� �ϴ��� ����
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

	//PlayerController�� ����
	SetInputMode(InputModeData);
	//���콺Ŀ�� On/Off
	bShowMouseCursor = true;
}

void ADBPlayerController::CollapseWidget(FString WidgetName)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Widget"));

	if (WidgetName.Equals(TEXT("Inventory")))
	{
		DBInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//��� UI������ ����Ǿ��� ��
	if (!CheckWidgetVisible())
	{
		bIsRecentlyWidgetCollapsed = true;
		SetNextTickEnableCanWidgetCollapsed();

		FInputModeGameOnly InputModeData;

		//PlayerController�� ����
		SetInputMode(InputModeData);
		//���콺Ŀ�� On/Off
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
	//MultiUI ���� ��ȣ�ۿ�
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



