// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBNeedMaterialForItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameData/DBItemSingleton.h"

UDBNeedMaterialForItemWidget::UDBNeedMaterialForItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBNeedMaterialForItemWidget::NativeConstruct()
{
	NeedCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NeedCountText")));
	ensure(NeedCountText);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);

	CurrentItemCount = 0;
	NeedItemCount = 0;
}

void UDBNeedMaterialForItemWidget::SetBlockSetting(FName NewItemID, int NewCurrentItemCount, int NewNeedItemCount)
{
	//아이템 이미지 설정
	if (ItemImage)
	{
		ItemID = NewItemID;

		//Texture 불러오기
		FString NewTexturePath = UDBItemSingleton::Get().GetLoadCountableItemData(NewItemID)->TexturePath;
		UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *NewTexturePath);

		//Image 등록
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemTexture);
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}

	//아이템 갯수 및 필요 갯수 설정
	CurrentItemCount = NewCurrentItemCount;
	NeedItemCount = NewNeedItemCount;
	if (NeedCountText)
	{
		NeedCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentItemCount, NeedItemCount)));
	}
}

bool UDBNeedMaterialForItemWidget::CheckCanCreateItem()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Check MaterialItem Count : %d >= %d"), CurrentItemCount, NeedItemCount));
	return CurrentItemCount >= NeedItemCount;
}
