#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DBItemStat.generated.h"

USTRUCT(BlueprintType)
struct FDBItemStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDBItemStat() : Name(""), ItemID(""), ItemType(0), Rank(0.0f), MaxHp(0.0f), Attack(0.0f), Defense(0.0f), MoveSpeed(0.0f), TexturePath("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString TexturePath;
};
