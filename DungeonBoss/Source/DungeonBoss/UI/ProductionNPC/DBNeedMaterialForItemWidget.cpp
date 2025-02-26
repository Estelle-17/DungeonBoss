// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProductionNPC/DBNeedMaterialForItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UDBNeedMaterialForItemWidget::UDBNeedMaterialForItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBNeedMaterialForItemWidget::NativeConstruct()
{
	NeedCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NeedCountText")));
	ensure(NeedCountText);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);
}

void UDBNeedMaterialForItemWidget::SetBlockSetting(UTexture2D* NewItemTexture, int CurrenItemCount, int NeedItemCount)
{
	//������ �̹��� ����
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(NewItemTexture);
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}

	//������ ���� �� �ʿ� ���� ����
	if (NeedCountText)
	{
		NeedCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrenItemCount, NeedItemCount)));
	}
}

void UDBNeedMaterialForItemWidget::MakeItem(FName ItemID)
{

}
