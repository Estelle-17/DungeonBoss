// Fill out your copyright notice in the Description page of Project Settings.

#include "GM/DBStageMoveActor.h"
#include "Components/BoxComponent.h"
#include "DungeonBoss.h"
#include "Player/PlayerCharacter.h"
#include "UI/GM/DBMoveFieldWidget.h"
#include "Player/DBPlayerController.h"

// Sets default values
ADBStageMoveActor::ADBStageMoveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossStatue"));
	Body->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.05f));

	PlayerCheckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerCheckCollision"));

	RootComponent = Body;
	Floor->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);

	//SkeletalMesh Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Prefab/Boss/Boss.Boss'"));
	if (BodyMeshRef.Object)
	{
		Body->SetSkeletalMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (FloorMeshRef.Object)
	{
		Floor->SetStaticMesh(FloorMeshRef.Object);
	}

	Body->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	Floor->SetRelativeLocation(FVector3d(0.0f, 200.0f, 4.0f));
	Floor->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.05f));

	PlayerCheckCollision->AttachToComponent(Floor, FAttachmentTransformRules::KeepRelativeTransform);
	PlayerCheckCollision->SetRelativeLocation(FVector3d(0.0f, 0.0f, 1500.0f));
	PlayerCheckCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 50.0f));
	PlayerCheckCollision->SetCollisionProfileName(TEXT("CheckPlayerPreset"));

	PlayerCheckCollision->OnComponentBeginOverlap.AddDynamic(this, &ADBStageMoveActor::OnOverlapBegin);
	PlayerCheckCollision->OnComponentEndOverlap.AddDynamic(this, &ADBStageMoveActor::OnOverlapEnd);

	//Widget Setting
	static ConstructorHelpers::FClassFinder<UDBMoveFieldWidget> DBMoveFieldWidgetRef(TEXT("/Game/UI/GM/WBP_MoveField.WBP_MoveField_C"));
	if (DBMoveFieldWidgetRef.Class)
	{
		DBMoveFieldWidgetClass = DBMoveFieldWidgetRef.Class;
	}
}

// Called when the game starts or when spawned
void ADBStageMoveActor::BeginPlay()
{
	Super::BeginPlay();

	DBPlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!DBPlayerController)
	{
		return;
	}

	DBMoveFieldWidget = CreateWidget<UDBMoveFieldWidget>(DBPlayerController, DBMoveFieldWidgetClass);
	if (DBMoveFieldWidget)
	{
		DBMoveFieldWidget->AddToViewport();
		DBMoveFieldWidget->StageMoveActor = this;
		DBMoveFieldWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ADBStageMoveActor::MoveFieldWidgetOn()
{
	DBMoveFieldWidget->SetVisibility(ESlateVisibility::Visible);
	LoadWidget(DBMoveFieldWidget);
}

void ADBStageMoveActor::SetNextTickEnableCanWidgetCollapsed()
{
	//�̹� �� �����ӿ� Widget�� �������� ���
	//���� ƽ�� Widget�Է��� ������������ ������
	GetWorld()->GetTimerManager().SetTimerForNextTick([&]
		{
			bIsRecentlyWidgetCollapsed = false;
		});
}

void ADBStageMoveActor::LoadWidget(UUserWidget* UIWidget)
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

void ADBStageMoveActor::CollapseWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Widget"));

	DBMoveFieldWidget->SetVisibility(ESlateVisibility::Collapsed);

	//��� UI������ ����Ǿ��� ��
	bIsRecentlyWidgetCollapsed = true;
	SetNextTickEnableCanWidgetCollapsed();

	FInputModeGameOnly InputModeData;

	//PlayerController�� ����
	DBPlayerController->SetInputMode(InputModeData);
	//���콺Ŀ�� On/Off
	DBPlayerController->bShowMouseCursor = false;
}

void ADBStageMoveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains(FName(TEXT("Player"))))
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Find Player"));

		APlayerCharacter* NewPlayer = Cast<APlayerCharacter>(OtherActor);
		if (NewPlayer)
		{
			//������ �÷��̾ �����÷��̾��̸� ������ ������ ���� ���
			if (NewPlayer->IsLocallyControlled() && NewPlayer->HasAuthority())
			{
				UEnum* EnumPtr = StaticEnum<ENextBossSelect>();
				if (EnumPtr)
				{
					FString NextBossName = EnumPtr->GetNameStringByValue((int64)NextBossSelect);
					NewPlayer->OnUpdatePlayerDescription.Broadcast(FString::Printf(TEXT("Enter %s Field"), *NextBossName));
				}
				NewPlayer->DBStageMoveActor = this;
			}
			else
			{
				NewPlayer->OnUpdatePlayerDescription.Broadcast(FString::Printf(TEXT("Only Master Can Use it")));
			}
		}
	}
}

void ADBStageMoveActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains(FName(TEXT("Player"))))
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Exit Player"));

		APlayerCharacter* NewPlayer = Cast<APlayerCharacter>(OtherActor);
		if (NewPlayer)
		{
			NewPlayer->OnDisablePlayerDescription.Broadcast();
			NewPlayer->DBStageMoveActor = nullptr;
		}
	}
}