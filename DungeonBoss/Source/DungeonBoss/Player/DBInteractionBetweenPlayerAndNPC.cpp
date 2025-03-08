// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBInteractionBetweenPlayerAndNPC.h"
#include "Props/DBNPCBaseActor.h"
#include "Player/DBPlayerController.h"
#include "Components/SphereComponent.h"


UDBInteractionBetweenPlayerAndNPC::UDBInteractionBetweenPlayerAndNPC()
{
}

void UDBInteractionBetweenPlayerAndNPC::InteractionNPC()
{
	if (NPCActor != nullptr)
	{
		NPCActor->PlayerInteractionAction();
	}
}

void UDBInteractionBetweenPlayerAndNPC::PlayerSearchOverlapSetting(ADBPlayerBase* PlayerBase)
{
	PlayerBase->SearchCollision->OnComponentBeginOverlap.AddDynamic(this, &UDBInteractionBetweenPlayerAndNPC::OnSearchOverlapBegin);
	PlayerBase->SearchCollision->OnComponentEndOverlap.AddDynamic(this, &UDBInteractionBetweenPlayerAndNPC::OnSearchOverlapEnd);
}

void UDBInteractionBetweenPlayerAndNPC::OnSearchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�÷��̾ �����̸� �ֺ��� NPC���� ���������� Ȯ��
	if (OtherActor->Tags.Contains(FName(TEXT("MultiplayerNPC"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());

		//Ư�� ���� ���� �÷��̾ ���� ��� MultiUI�� �� �� �ִ� ������ ���� �˷���
		ADBNPCBaseActor* NPCBaseActor = Cast<ADBNPCBaseActor>(OtherActor);

		if (NPCBaseActor)
		{
			NPCActor = NPCBaseActor;
			NPCBaseActor->bIsPlayerInArea = true;
		}
	}
	else if (OtherActor->Tags.Contains(FName(TEXT("NPC"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());

		//Ư�� ���� ���� �÷��̾ ���� ��� ItemMakingNPCUI�� �� �� �ִ� ������ ���� �˷���
		ADBNPCBaseActor* NPCBaseActor = Cast<ADBNPCBaseActor>(OtherActor);

		if (NPCBaseActor)
		{
			NPCActor = NPCBaseActor;
			NPCBaseActor->bIsPlayerInArea = true;
		}
	}
}

void UDBInteractionBetweenPlayerAndNPC::OnSearchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//�÷��̾ �����̸� �ֺ��� NPC���� ���������� Ȯ��
	if (OtherActor->Tags.Contains(FName(TEXT("MultiplayerNPC"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());

		//Ư�� ���� ���� �÷��̾ ���� ��� MultiUI�� �� �� �ִ� ������ ���� �˷���
		ADBNPCBaseActor* NPCBaseActor = Cast<ADBNPCBaseActor>(OtherActor);

		if (NPCBaseActor)
		{
			NPCActor = nullptr;
			NPCBaseActor->bIsPlayerInArea = false;
		}
	}
	else if (OtherActor->Tags.Contains(FName(TEXT("NPC"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Search : %s"), *OtherActor->GetFName().ToString());

		//Ư�� ���� ���� �÷��̾ ���� ��� ItemMakingNPCUI�� �� �� �ִ� ������ ���� �˷���
		ADBNPCBaseActor* NPCBaseActor = Cast<ADBNPCBaseActor>(OtherActor);

		if (NPCBaseActor)
		{
			NPCActor = nullptr;
			NPCBaseActor->bIsPlayerInArea = false;
		}
	}
}