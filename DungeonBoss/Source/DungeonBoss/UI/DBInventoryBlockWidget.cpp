// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DBInventoryBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

UDBInventoryBlockWidget::UDBInventoryBlockWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDBInventoryBlockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	ensure(ItemCount);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ensure(ItemImage);

	//EquipCheck Setting
	EquipCheck = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("EquipCheckImage")));
	ensure(EquipCheck);
	EquipCheck->SetVisibility(ESlateVisibility::Collapsed);
}

void UDBInventoryBlockWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	if (!CountableItemData && !EquipItemData)
	{
		CountableItemData = Cast<UDBCountableItemData>(ListItemObject);
		if (CountableItemData)
		{
			CountableItemData->OnSetItemCount.AddUObject(this, &UDBInventoryBlockWidget::SetCountableItemSetting);
		}

		EquipItemData = Cast<UDBEquipItemData>(ListItemObject);
	}

	if (CountableItemData)
	{
		SetCountableItemSetting();
	}
	else
	{
		SetEquipItemSetting();
	}
}

void UDBInventoryBlockWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	if (bIsSelected)
	{
		UE_LOG(LogTemp, Log, TEXT("%s is Select true"), *this->GetFName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s is Select false"), *this->GetFName().ToString());
	}
	
}

FReply UDBInventoryBlockWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//우클릭 입력이 들어왔을 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Is Clicked RightMouseButton"));
		//장비 아이템만 장착 가능
		if (EquipItemData)
		{
			EquipCheck->SetVisibility(ESlateVisibility::Visible);
			//인벤토리 스크립트에 아이템 정보 보낸 후 장비 창에 이미지 등록
			//이후 아이템에 맞춰 스탯 설정
		}
	}

	return Reply.NativeReply;
}

void UDBInventoryBlockWidget::SetEquipItemSetting()
{
	ensure(EquipItemData);

	ItemCount->SetText(FText::FromString(TEXT("")));
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(EquipItemData->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}

void UDBInventoryBlockWidget::SetCountableItemSetting()
{
	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), CountableItemData->GetItenCount())));
	}
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(CountableItemData->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}
