#pragma once

#include "CoreMinimal.h"
#include "Item/DBItemObject.h"
#include "DBCharacterEquipItems.generated.h"

USTRUCT(BlueprintType)
struct FDBCharacterEquipItems
{
	GENERATED_BODY()

public:
	FDBCharacterEquipItems() : WeaponEquipItem(nullptr), HeadEquipItem(nullptr), BodyEquipItem(nullptr), ShoesEquipItem(nullptr){}

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* WeaponEquipItem;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* HeadEquipItem;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* BodyEquipItem;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UDBItemObject* ShoesEquipItem;
};
