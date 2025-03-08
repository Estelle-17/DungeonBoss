// Fill out your copyright notice in the Description page of Project Settings.

#include "GM/DBNetworkSetting.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

ADBNetworkSetting::ADBNetworkSetting()
	: OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
	, OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{

	/*// OnlineSubsystem ����
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
	}*/
}

// Called when the game starts or when spawned
void ADBNetworkSetting::BeginPlay()
{
	Super::BeginPlay();
}

void ADBNetworkSetting::CreateRoomSection()
{
	UWorld* World = GetWorld();
	if (World)
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

		World->ServerTravel("/Game/Maps/Map1?listen");
	}

	/*
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
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);

	UE_LOG(LogTemp, Log, TEXT("Finish SessionSetting"));
	*/
}

void ADBNetworkSetting::DestroyRoomSection()
{
	////OnlineSessionInterface�� �����ϴ��� Ȯ��
	//if (!OnlineSessionInterface.IsValid())
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed")));
	//	}
	//	return;
	//}

	////������ ������ �� ���� ���� ����
	//auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	//if (ExistingSession != nullptr)
	//{
	//	// Log
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("Destroy session : %s"), NAME_GameSession));
	//	}

	//	OnlineSessionInterface->DestroySession(NAME_GameSession);

	//	UE_LOG(LogTemp, Log, TEXT("Finish DestroySession"));
	//}
}

void ADBNetworkSetting::FindRoomSection(FString Address)
{
	if (Address == "")
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("IP is invailed")));
		}
	}
	else
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

			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}

	////OnlineSessionInterface�� �����ϴ��� Ȯ��
	//if (!OnlineSessionInterface.IsValid())
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed")));
	//	}
	//	return;
	//}

	////���� ���� �Ϸ� �� ȣ��� delegate����Ʈ�� FindSessionComplete �߰�
	//OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	//// Find Game Session
	//SessionSearch = MakeShareable(new FOnlineSessionSearch());
	//SessionSearch->MaxSearchResults = 10000;	// �˻� ����� ������ ���� �� �ִ밪
	//SessionSearch->bIsLanQuery = false;			// LAN ��� ����
	//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // ã�� ���� ������ ����� �����Ѵ�

	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void ADBNetworkSetting::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//���� ���� ���� �� ���� �޼��� ���
	if (bWasSuccessful)
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

void ADBNetworkSetting::OnFindSessionsComplete(bool bWasSuccessful)
{
	//���� ã�µ� �����ߴٸ� return
	if (!OnlineSessionInterface.IsValid()
		|| !bWasSuccessful)
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString(TEXT("======== Search Result ========")));
		if (SessionSearch)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Find Session : %d"), SessionSearch->SearchResults.Num()));
		}
	}

	for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;

		//��ġ Ÿ�� Ȯ���ϱ�
		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		//ã�� ������ ���� ����ϱ�
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Session ID : %s / Owner : %s"), *Id, *User));
		}

		//������ ��ġ Ÿ���� �˻� ����� ���� ��� ���� ����(���� FreeForAllŸ���� �� ���� ����
		if (MatchType == FString("FreeForAll"))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Joining Match Type : %s"), *MatchType));
			}

			// Join Session Complete Delegate ��� 
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

void ADBNetworkSetting::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//OnlineSessionInterface�� �����ϴ��� Ȯ��
	if (!OnlineSessionInterface.IsValid())
		return;

	//���ǿ� �����ߴٸ� IP Address���ͼ� �ش� ������ ����
	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *Address));
		}

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			//������ ���� �� ��� UI�� ���ŵǹǷ� InputMode����
			FInputModeGameOnly InputModeData;
			//PlayerController�� ����
			PlayerController->SetInputMode(InputModeData);
			//���콺Ŀ�� On/Off
			PlayerController->bShowMouseCursor = false;

			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}

