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
	//������ �̹��� ����
	if (ItemImage)
	{
		ItemID = NewItemID;

		//Texture �ҷ�����
		FString NewTexturePath = UDBItemSingleton::Get().GetLoadCountableItemData(NewItemID)->TexturePath;
		UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *NewTexturePath);

		//Image ���
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemTexture);
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}

	//������ ���� �� �ʿ� ���� ����
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
