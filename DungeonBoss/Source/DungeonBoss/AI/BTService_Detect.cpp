// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "DBAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DBAI.h"
#include "DrawDebugHelpers.h"
#include "Interface/DBEnemyAIInterface.h"
#include "Engine/OverlapResult.h"
#include "Enemy/DBEnemyBase.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//제어하고 있는 폰의 정보를 가져옴
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	IDBEnemyAIInterface* AIPawn = Cast<IDBEnemyAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	//근처의 플레이어들을 탐지함
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_GameTraceChannel10,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	//탐지 범위를 디버드 동안 육안으로 확인 가능하도록 설정
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Detect Player : %s"), *Pawn->GetName()));

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				ADBEnemyBase* EnemyBase = Cast<ADBEnemyBase>(ControllingPawn);
				if (EnemyBase)
				{
					EnemyBase->TargetPawn = Pawn;
				}

				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
