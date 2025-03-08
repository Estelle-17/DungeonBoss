// Fill out your copyright notice in the Description page of Project Settings.

#include "Props/DBNPCBaseActor.h"
#include "UI/DBMultiUIWidget.h"
#include "UI/ProductionNPC/DBProductionNPCWidget.h"
#include "Player/DBPlayerController.h"

// Sets default values
ADBNPCBaseActor::ADBNPCBaseActor()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	RootComponent = Body;

	//Mesh Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UDBMultiUIWidget> DBMultiUIWidgetRef(TEXT("/Game/UI/WBP_DBMultiUI.WBP_DBMultiUI_C"));
	if (DBMultiUIWidgetRef.Class)
	{
		DBMultiUIWidgetClass = DBMultiUIWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UDBProductionNPCWidget> DBProductionNPCWidgetRef(TEXT("/Game/UI/NPC/WBP_EquipNPC.WBP_EquipNPC_C"));
	if (DBProductionNPCWidgetRef.Class)
	{
		DBProductionNPCWidgetClass = DBProductionNPCWidgetRef.Class;
	}

	NPCType = ENPCType::ItemMakingNPC;
	Tags.Add(FName("NPC"));

	bIsPlayerInArea = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADBNPCBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	DBPlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());

	if(!DBPlayerController)
	{
		return;
	}

	//UI Setting
	switch (NPCType)
	{
		case ENPCType::MultiplayerNPC:
			DBMultiUIWidget = CreateWidget<UDBMultiUIWidget>(DBPlayerController, DBMultiUIWidgetClass);
			if (DBMultiUIWidget)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Check : %s"), *DBMultiUIWidget->GetFName().ToString()));
				DBMultiUIWidget->AddToViewport();
				DBMultiUIWidget->BindingButtons(DBPlayerController->GetNetworkSetting());
				DBMultiUIWidget->NPCBaseActor = this;
				DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case ENPCType::ItemMakingNPC:
			DBProductionNPCWidget = CreateWidget<UDBProductionNPCWidget>(DBPlayerController, DBProductionNPCWidgetClass);
			if (DBProductionNPCWidget)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Check : %s"), *DBProductionNPCWidget->GetFName().ToString()));
				DBProductionNPCWidget->SetAllMenuUI();
				DBProductionNPCWidget->AddToViewport();
				DBProductionNPCWidget->NPCBaseActor = this;
				DBProductionNPCWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		default:
			break;
	}
}

void ADBNPCBaseActor::SetNextTickEnableCanWidgetCollapsed()
{
	//�̹� �� �����ӿ� Widget�� �������� ���
	//���� ƽ�� Widget�Է��� ������������ ������
	GetWorld()->GetTimerManager().SetTimerForNextTick([&]
		{
			bIsRecentlyWidgetCollapsed = false;
		});
}

void ADBNPCBaseActor::LoadWidget(UUserWidget* UIWidget)
{
	bIsRecentlyWidgetCollapsed = true;
	SetNextTickEnableCanWidgetCollapsed();

	FInputModeUIOnly InputModeData;
	//����� ���� ����
	InputModeData.SetWidgetToFocus(UIWidget->TakeWidget());
	//���콺�� Viewport�� �����Ұ���, �����Ӱ� �ϴ��� ����
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

	//PlayerController�� ����
	DBPlayerController->SetInputMode(InputModeData);
	//���콺Ŀ�� On/Off
	DBPlayerController->bShowMouseCursor = true;
}

void ADBNPCBaseActor::CollapseWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Widget"));

	//��� UI������ ����Ǿ��� ��
	bIsRecentlyWidgetCollapsed = true;
	SetNextTickEnableCanWidgetCollapsed();

	FInputModeGameOnly InputModeData;

	//PlayerController�� ����
	DBPlayerController->SetInputMode(InputModeData);
	//���콺Ŀ�� On/Off
	DBPlayerController->bShowMouseCursor = false;
}

void ADBNPCBaseActor::PlayerInteractionAction()
{
	if (bIsPlayerInArea)
	{
		switch (NPCType)
		{
			case ENPCType::MultiplayerNPC:
				if (DBMultiUIWidget->IsVisible())
				{
					DBMultiUIWidget->SetVisibility(ESlateVisibility::Collapsed);
					CollapseWidget();
				}
				else
				{
					DBMultiUIWidget->SetVisibility(ESlateVisibility::Visible);
					LoadWidget(CastChecked<UUserWidget>(DBMultiUIWidget));
				}
				break;
			case ENPCType::ItemMakingNPC:
				if (DBProductionNPCWidget->IsVisible())
				{
					DBProductionNPCWidget->SetVisibility(ESlateVisibility::Collapsed);
					CollapseWidget();
				}
				else
				{
					DBProductionNPCWidget->SetVisibility(ESlateVisibility::Visible);
					DBProductionNPCWidget->InventoryWidget = DBPlayerController->GetInventoryWidget();
					LoadWidget(CastChecked<UUserWidget>(DBProductionNPCWidget));
				}
				break;
			default:
				break;
		}
	}
}
