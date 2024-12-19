// Fill out your copyright notice in the Description page of Project Settings.

#include "GM/DBNetworkSetting.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

ADBNetworkSetting::ADBNetworkSetting() :
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{

	// OnlineSubsystem ����
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		// �¶��� ���� �޾ƿ���
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			//OnlineSubsystem �̸� ����ϱ�
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}
}

// Called when the game starts or when spawned
void ADBNetworkSetting::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADBNetworkSetting::CreateRoomSection()
{
	//OnlineSessionInterface�� �����ϴ��� Ȯ��
	if (!OnlineSessionInterface.IsValid())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed")));
		}
		return;
	}

	//�̹� ������ �����Ѵٸ� ���� ���� ����
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		// Log
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("Destroy session : %s"), NAME_GameSession));
		}

		OnlineSessionInterface->DestroySession(NAME_GameSession);

		UE_LOG(LogTemp, Log, TEXT("Finish DestroySession"));
	}

	//���� ���� �Ϸ� �� ȣ��� delegate����Ʈ�� OnCreateSessionComplete �߰�
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	//���� ����
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = false;			// LAN ����
	SessionSettings->NumPublicConnections = 4;		// �ִ� ���� ���� ��
	SessionSettings->bAllowJoinInProgress = true;	// Session �����߿� ���� ���
	SessionSettings->bAllowJoinViaPresence = true;	// ���� ���� ������ ���� �������� ���� (������ presence ���)
	SessionSettings->bShouldAdvertise = true;		// ���� ������ �������� (������ �ٸ� �÷��̾�� ���� ȫ�� ����)
	SessionSettings->bUsesPresence = true;			// ���� ������ ���� ǥ��
	SessionSettings->bUseLobbiesIfAvailable = true; // �÷����� �����ϴ� ��� �κ� API ���

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);

	UE_LOG(LogTemp, Log, TEXT("Finish SessionSetting"));

}

void ADBNetworkSetting::DestroyRoomSection()
{
	//������ ������ �� ���� ���� ����
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		// Log
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("Destroy session : %s"), NAME_GameSession));
		}

		OnlineSessionInterface->DestroySession(NAME_GameSession);

		UE_LOG(LogTemp, Log, TEXT("Finish DestroySession"));
	}
}

void ADBNetworkSetting::FindRoomSection()
{

}

void ADBNetworkSetting::OnCreateSessionComplete(FName SessionName, bool bWasSuccssful)
{
	//���� ���� ���� �� ���� �޼��� ���
	if (bWasSuccssful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Created session : %s"), *SessionName.ToString()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to create session!")));
		}
	}
}

