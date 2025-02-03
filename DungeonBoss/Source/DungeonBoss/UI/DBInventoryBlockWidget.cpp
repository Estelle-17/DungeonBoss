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

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("InventoryImage")));
	ensure(ItemImage);

	FString ItemSlotTexturePtr = "/Script/Engine.Texture2D'/Game/Texture/Items/Equip/InventoryBlockTexture.InventoryBlockTexture'";
	ItemSlotTexture = LoadObject<UTexture2D>(nullptr, *ItemSlotTexturePtr);
}

void UDBInventoryBlockWidget::SetItemSetting(UObject* ItemObject)
{
	ItemObjectData = Cast<UDBItemObject>(ItemObject);
	if (!ItemObjectData)
	{
		return;
	}

	if (ItemObjectData->bIsCountableItem)
	{
		SetCountableItemSetting();
	}
	else
	{
		SetEquipItemSetting();
	}
}

void UDBInventoryBlockWidget::SetEquipItemSetting()
{
	ItemCount->SetText(FText::FromString(TEXT("")));
	if (ItemImage && ItemObjectData)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemObjectData->GetEquipItemData()->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}

void UDBInventoryBlockWidget::SetCountableItemSetting()
{
	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemObjectData->GetItemCount())));
	}
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemObjectData->GetCountableItemData()->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}

FReply UDBInventoryBlockWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!ItemObjectData)
	{
		return Reply.NativeReply;
	}

	//��Ŭ�� �Է��� ������ ���
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Is Clicked RightMouseButton"));
		//��� �����۸� ���� ����
		if (!ItemObjectData->bIsCountableItem)
		{
			//ItemData�� ������ ������ ������� �˷���
			ItemObjectData->SetPlayerEquipSetting();
		}
	}

	return Reply.NativeReply;
}