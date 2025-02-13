// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBItemDragVisualWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Texture2D.h"

UDBItemDragVisualWidget::UDBItemDragVisualWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBItemDragVisualWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);
}

void UDBItemDragVisualWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag Tick"));

	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
	FVector2D finalPos{ MousePos.X - (CurrentImageSize.X / 2), MousePos.Y - (CurrentImageSize.Y / 2) };

	ItemImage->SetRenderTranslation(finalPos);
}

void UDBItemDragVisualWidget::SetItemImage(FVector2D ImageSize)
{
	ItemImage->SetDesiredSizeOverride(ImageSize);

	FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
	ItemImage->SetBrushFromTexture(ItemTexture);
	ItemImage->SetDesiredSizeOverride(CurrentImageSize);
}
