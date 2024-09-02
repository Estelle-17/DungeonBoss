// Fill out your copyright notice in the Description page of Project Settings.


#include "DBGameMode.h"
#include "DungeonBoss.h"
#include "DBGameState.h"

ADBGameMode::ADBGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef = TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'");
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/DungeonBoss.DBPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = ADBGameState::StaticClass();
}

//클라이언트의 접속 요청을 처리하는 함수
/*void ADBGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("========================================================"));
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	//ErrorMessage = TEXT("Server Is Full");

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

//접속을 허용한 클라이언트에 대응하는 플레이어 컨트롤러를 만드는 함수
APlayerController* ADBGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

//플레이어 입장을 위해 플레이어에 필요한 기본 설정을 마무리하는 함수
void ADBGameMode::PostLogin(APlayerController* NewPlayer)
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();

	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				DB_LOG(LogDBNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

//게임의 시작을 지시하는 함수
void ADBGameMode::StartPlay()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}*/
