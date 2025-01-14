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

	//��Ŭ�� �Է��� ������ ���
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Is Clicked RightMouseButton"));
		//��� �����۸� ���� ����
		if (EquipItemData)
		{
			EquipCheck->SetVisibility(ESlateVisibility::Visible);
			//�κ��丮 ��ũ��Ʈ�� ������ ���� ���� �� ��� â�� �̹��� ���
			//���� �����ۿ� ���� ���� ����
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
