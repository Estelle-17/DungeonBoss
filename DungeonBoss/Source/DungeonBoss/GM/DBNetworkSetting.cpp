// Fill out your copyright notice in the Description page of Project Settings.

#include "GM/DBNetworkSetting.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

ADBNetworkSetting::ADBNetworkSetting() :
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{

	// OnlineSubsystem 연결
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
	}
}

// Called when the game starts or when spawned
void ADBNetworkSetting::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADBNetworkSetting::CreateRoomSection()
{
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

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);

	UE_LOG(LogTemp, Log, TEXT("Finish SessionSetting"));

}

void ADBNetworkSetting::DestroyRoomSection()
{
	//세션이 존재할 때 기존 세션 삭제
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
	//세션 생성 성공 및 실패 메세지 출력
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

