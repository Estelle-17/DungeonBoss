// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBMultiUIWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DungeonBoss.h"
#include "Kismet/GameplayStatics.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Player/DBPlayerController.h"
#include "Props/DBNPCBaseActor.h"

UDBMultiUIWidget::UDBMultiUIWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBMultiUIWidget::NativeConstruct()
{
	PrivateCheck = Cast<UCheckBox>(GetWidgetFromName(TEXT("PrivateCheck")));
	ensure(PrivateCheck);
	
	CreateRoomText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("CreateRoomNameTextBox")));
	ensure(CreateRoomText);
	
	JoinRoomText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("JoinRoomNameTextBox")));
	ensure(JoinRoomText);

	CreateRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateRoomButton")));
	ensure(CreateRoomButton);

	DestroyRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("DestroyRoomButton")));
	ensure(DestroyRoomButton);

	JoinRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinRoomButton")));
	ensure(JoinRoomButton);

	JoinRandomRoomButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinRandomRoomButton")));
	ensure(JoinRandomRoomButton);

	CheckIPButton = Cast<UButton>(GetWidgetFromName(TEXT("CheckIPButton")));
	ensure(CheckIPButton);
	if (CheckIPButton)
	{
		CheckIPButton->OnClicked.AddDynamic(this, &UDBMultiUIWidget::CoverImageOnOff);
	}

	CoverImage = Cast<UImage>(GetWidgetFromName(TEXT("CoverImage")));
	ensure(CoverImage);

	AddressText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AddressText")));
	ensure(AddressText);

	CheckIPButtonText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CheckIPButtonText")));
	ensure(CheckIPButtonText);
}

void UDBMultiUIWidget::BindingButtons(ADBNetworkSetting* NetworkSetting)
{
	CreateRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::CreateRoomSection);
	DestroyRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::DestroyRoomSection);
	JoinRoomButton->OnClicked.AddDynamic(this, &UDBMultiUIWidget::JoinGameServer);
	JoinRandomRoomButton->OnClicked.AddDynamic(this, &UDBMultiUIWidget::JoinGameServer);
	OnJoinGameServerDelegate.AddUObject(NetworkSetting, &ADBNetworkSetting::FindRoomSection);

	SetPlayerLocalAddress();
}

void UDBMultiUIWidget::JoinGameServer()
{
	OnJoinGameServerDelegate.Broadcast(JoinRoomText->GetText().ToString());
}

//로컬 플레이어의 IP확인
void UDBMultiUIWidget::SetPlayerLocalAddress()
{
	bool CanBind = false;
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBind);

	if (Addr.IsValid())
	{
		FString LocalIP = Addr->ToString(false);
		AddressText->SetText(FText::FromString(LocalIP));
	}
}

void UDBMultiUIWidget::CoverImageOnOff()
{
	if (CoverImage->GetVisibility() == ESlateVisibility::Visible)
	{
		CoverImage->SetVisibility(ESlateVisibility::Collapsed);
		CheckIPButtonText->SetText(FText::FromString(TEXT("Hide IP")));
	}
	else if (CoverImage->GetVisibility() == ESlateVisibility::Collapsed)
	{
		CoverImage->SetVisibility(ESlateVisibility::Visible);
		CheckIPButtonText->SetText(FText::FromString(TEXT("Check IP")));
	}
}

FReply UDBMultiUIWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Collapse Inventory"));

		NPCBaseActor->PlayerInteractionAction();
	}

	return Reply.NativeReply;
}
