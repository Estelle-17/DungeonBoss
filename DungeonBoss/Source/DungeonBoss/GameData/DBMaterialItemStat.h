#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DBMaterialItemStat.generated.h"

USTRUCT(BlueprintType)
struct FDBMaterialItemStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDBMaterialItemStat() : Name(""), ItemType(0), Rank(0.0f), MinCount(0.0f), MaxCount(0.0f), TexturePath("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int32 ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString TexturePath;
};
