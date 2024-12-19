// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBMultiUIWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "DungeonBoss.h"

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
}

void UDBMultiUIWidget::BindingButtons(ADBNetworkSetting* NetworkSetting)
{
	CreateRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::CreateRoomSection);
	DestroyRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::DestroyRoomSection);
	JoinRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::FindRoomSection);
	JoinRandomRoomButton->OnClicked.AddDynamic(NetworkSetting, &ADBNetworkSetting::FindRoomSection);
}
