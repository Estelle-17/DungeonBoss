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

	/*// OnlineSubsystem 연결
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		// 온라인 세션 받아오기
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			//OnlineSubsystem 이름 출력하기
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
			//서버에 접속 시 모든 UI가 제거되므로 InputMode변경
			FInputModeGameOnly InputModeData;
			//PlayerController에 적용
			PlayerController->SetInputMode(InputModeData);
			//마우스커서 On/Off
			PlayerController->bShowMouseCursor = false;
		}

		World->ServerTravel("/Game/Maps/Map1?listen");
	}

	/*
	//OnlineSessionInterface가 존재하는지 확인
	if (!OnlineSessionInterface.IsValid())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed")));
		}
		return;
	}

	//이미 세션이 존재한다면 기존 세션 삭제
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

	//세션 생성 완료 후 호출될 delegate리스트에 OnCreateSessionComplete 추가
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	//세션 세팅
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = false;			// LAN 연결
	SessionSettings->NumPublicConnections = 4;		// 최대 접속 가능 수
	SessionSettings->bAllowJoinInProgress = true;	// Session 진행중에 접속 허용
	SessionSettings->bAllowJoinViaPresence = true;	// 세션 참가 지역을 현재 지역으로 제한 (스팀의 presence 사용)
	SessionSettings->bShouldAdvertise = true;		// 현재 세션을 광고할지 (스팀의 다른 플레이어에게 세션 홍보 여부)
	SessionSettings->bUsesPresence = true;			// 현재 지역에 세션 표시
	SessionSettings->bUseLobbiesIfAvailable = true; // 플랫폼이 지원하는 경우 로비 API 사용
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);

	UE_LOG(LogTemp, Log, TEXT("Finish SessionSetting"));
	*/
}

void ADBNetworkSetting::DestroyRoomSection()
{
	////OnlineSessionInterface가 존재하는지 확인
	//if (!OnlineSessionInterface.IsValid())
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed")));
	//	}
	//	return;
	//}

	////세션이 존재할 때 기존 세션 삭제
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
			//서버에 접속 시 모든 UI가 제거되므로 InputMode변경
			FInputModeGameOnly InputModeData;
			//PlayerController에 적용
			PlayerController->SetInputMode(InputModeData);
			//마우스커서 On/Off
			PlayerController->bShowMouseCursor = false;

			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}

	////OnlineSessionInterface가 존재하는지 확인
	//if (!OnlineSessionInterface.IsValid())
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed")));
	//	}
	//	return;
	//}

	////세션 생성 완료 후 호출될 delegate리스트에 FindSessionComplete 추가
	//OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	//// Find Game Session
	//SessionSearch = MakeShareable(new FOnlineSessionSearch());
	//SessionSearch->MaxSearchResults = 10000;	// 검색 결과로 나오는 세션 수 최대값
	//SessionSearch->bIsLanQuery = false;			// LAN 사용 여부
	//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // 찾을 세션 쿼리를 현재로 설정한다

	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void ADBNetworkSetting::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//세션 생성 성공 및 실패 메세지 출력
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
	//방을 찾는데 실패했다면 return
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

		//매치 타입 확인하기
		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		//찾은 세션의 정보 출력하기
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Session ID : %s / Owner : %s"), *Id, *User));
		}

		//세션의 매치 타입이 검색 결과와 같은 경우 세션 참가(현재 FreeForAll타입일 때 세션 참가
		if (MatchType == FString("FreeForAll"))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Joining Match Type : %s"), *MatchType));
			}

			// Join Session Complete Delegate 등록 
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

void ADBNetworkSetting::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//OnlineSessionInterface가 존재하는지 확인
	if (!OnlineSessionInterface.IsValid())
		return;

	//세션에 조인했다면 IP Address얻어와서 해당 서버에 접속
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
			//서버에 접속 시 모든 UI가 제거되므로 InputMode변경
			FInputModeGameOnly InputModeData;
			//PlayerController에 적용
			PlayerController->SetInputMode(InputModeData);
			//마우스커서 On/Off
			PlayerController->bShowMouseCursor = false;

			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}

