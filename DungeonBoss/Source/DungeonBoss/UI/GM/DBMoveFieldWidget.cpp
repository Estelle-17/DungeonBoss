// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GM/DBMoveFieldWidget.h"
#include "GM/DBStageMoveActor.h"
#include "Components/Button.h"

UDBMoveFieldWidget::UDBMoveFieldWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBMoveFieldWidget::NativeConstruct()
{
	SetIsFocusable(true);

	YesButton = Cast<UButton>(GetWidgetFromName(TEXT("YesButton")));
	ensure(YesButton);
	YesButton->OnClicked.AddDynamic(this, &UDBMoveFieldWidget::MoveField);

	NoButton = Cast<UButton>(GetWidgetFromName(TEXT("NoButton")));
	ensure(NoButton);
	NoButton->OnClicked.AddDynamic(this, &UDBMoveFieldWidget::DisableWidget);
}

void UDBMoveFieldWidget::MoveField()
{
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController)
	{
		//������ ���� �� ��� UI�� ���ŵǹǷ� InputMode����
		FInputModeGameOnly InputModeData;
		//PlayerController�� ����
		PlayerController->SetInputMode(InputModeData);
		//���콺Ŀ�� On/Off
		PlayerController->bShowMouseCursor = false;
	}
	GetWorld()->ServerTravel("/Game/Maps/BossMap?listen");
}

void UDBMoveFieldWidget::DisableWidget()
{
	if (StageMoveActor)
	{
		StageMoveActor->CollapseWidget();
	}
}

FReply UDBMoveFieldWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse MoveFieldWidget"));

		StageMoveActor->CollapseWidget();
	}

	return Reply.NativeReply;
}
