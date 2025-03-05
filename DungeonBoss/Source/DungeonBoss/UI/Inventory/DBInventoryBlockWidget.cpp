// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/DBInventoryBlockWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Item/ItemDragDropOperation.h"
#include "Player/DBPlayerController.h"
#include "UI/Inventory/DBItemDragVisualWidget.h"
#include "UI/Inventory/DBItemCountScrollBarWidget.h"

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

	ItemEnterImage = Cast<UImage>(GetWidgetFromName(TEXT("InventoryEnterImage")));
	ensure(ItemEnterImage);
	ItemEnterImage->SetVisibility(ESlateVisibility::Collapsed);

	TranslucnetImage = Cast<UImage>(GetWidgetFromName(TEXT("TranslucentImage")));
	ensure(TranslucnetImage);
	TranslucnetImage->SetVisibility(ESlateVisibility::Collapsed);

	ItemSlotType = EItemSlotType::None;
	bIsEquipSlot = false;

	PlayerController = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayerContorller에서 ItemDragVisualWidget가져오기
	if (PlayerController)
	{
		ItemDragVisualWidget = PlayerController->GetItemDragVisualWidget();
		ItemCountScrollBarWidget = PlayerController->GetItemCountScrollBarWidget();
	}
}

void UDBInventoryBlockWidget::SetItemSetting(UObject* ItemObject)
{
	ItemObjectData = Cast<UDBItemObject>(ItemObject);
	if (!ItemObjectData)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Success InventoryBlock In ItemObjectData!"));

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
	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(TEXT("")));
	}
	if (ItemImage && ItemObjectData)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemObjectData->GetEquipItemData()->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}

void UDBInventoryBlockWidget::SetCountableItemSetting()
{
	//수량이 변경되었을 때 숫자 변경을 위한 Delegate에 함수 등록
	ItemObjectData->OnSetItemCount.Clear();
	ItemObjectData->OnSetItemCount.AddUObject(this, &UDBInventoryBlockWidget::SetCountableItemSetting);

	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemObjectData->GetItemCount())));
	}
	if (ItemImage && ItemObjectData)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemObjectData->GetCountableItemData()->GetItemTexture());
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}
}

void UDBInventoryBlockWidget::ResetInventorySlot()
{
	ItemObjectData = nullptr;

	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(TEXT("")));
	}
	if (ItemImage)
	{
		FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
		ItemImage->SetBrushFromTexture(ItemSlotTexture);
		ItemImage->SetDesiredSizeOverride(CurrentImageSize);
	}

	ItemSlotType = EItemSlotType::None;
}

void UDBInventoryBlockWidget::SetTranslucnetImageDisable()
{
	TranslucnetImage->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma region Input Setting

FReply UDBInventoryBlockWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!ItemObjectData)
	{
		return Reply.NativeReply;
	}

	//좌클릭 입력이 들어왔을 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		//만약 Ctrl키도 같이 눌렸다면
		if (InMouseEvent.IsControlDown() && ItemObjectData->bIsCountableItem)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Divide Item"));

			//SetKeyboardFocus()
			
			//Widget위치 변경
			OnDivideItemSlider.Broadcast(ItemObjectData, ItemObjectData->GetItemCount());
		}
		else if (ItemSlotType == EItemSlotType::Item)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag : LeftMouseButton"));

			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}

	//우클릭 입력이 들어왔을 경우
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Is Clicked RightMouseButton"));
		//장비 아이템만 장착 가능
		//SlotNumber 80~83은 장착아이템 칸 번호
		if (!ItemObjectData->bIsCountableItem)
		{
			if (SlotNumber < 80)
			{
				//장착 아이템을 장비 칸으로 이동
				OnEquipItems.Broadcast(ItemObjectData, SlotNumber, ItemObjectData->GetEquipItemData()->EquipItemType, ItemSlotType);

				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Equip Success"));
			}
			else
			{
				OnMoveEquipItemToEmptySlot.Broadcast(ItemObjectData, SlotNumber);

				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Release Equip Success"));
			}
		}
	}

	return Reply.NativeReply;
}

void UDBInventoryBlockWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	ItemEnterImage->SetVisibility(ESlateVisibility::Visible);
}

void UDBInventoryBlockWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	ItemEnterImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UDBInventoryBlockWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag Start"));

		TranslucnetImage->SetVisibility(ESlateVisibility::HitTestInvisible);

		UItemDragDropOperation* ItemDragDropOperation = NewObject<UItemDragDropOperation>();

		OutOperation = ItemDragDropOperation;
		ItemDragDropOperation->SlotNumber = SlotNumber;
		ItemDragDropOperation->SlotType = ItemSlotType;

		//드래그 시 보여줄 이미지 설정
		if (ItemDragVisualWidget)
		{
			ItemDragVisualWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (ItemObjectData->bIsCountableItem)
			{
				ItemDragVisualWidget->ItemTexture = ItemObjectData->GetCountableItemData()->GetItemTexture();
			}
			else
			{
				ItemDragVisualWidget->ItemTexture = ItemObjectData->GetEquipItemData()->GetItemTexture();
			}
			FVector2D CurrentImageSize = ItemImage->GetBrush().ImageSize;
			ItemDragVisualWidget->SetItemImage(CurrentImageSize / 0.8f);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag failed"));
	}
}

void UDBInventoryBlockWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	//드래그 시 보여줄 이미지 설정
	if (ItemDragVisualWidget)
	{
		ItemDragVisualWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//드래그 시작한 아이템 이미지 다시 밝게 변경
	SetTranslucnetImageDisable();

	//UDragDropOperation 데이터 삭제
	InOperation = nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag Cancelled"));
}

bool UDBInventoryBlockWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag End"));

	if (ItemDragDropOperation != nullptr)
	{
		//드래그 시 보여줄 이미지 설정
		if (ItemDragVisualWidget)
		{
			ItemDragVisualWidget->ItemTexture = nullptr;
			ItemDragVisualWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		//제자리에 다시 드롭했을 시
		if (ItemDragDropOperation->SlotNumber == SlotNumber)
		{
			return false;
		}
		//아이템 위치 변경
		OnDragSwapItems.Broadcast(ItemDragDropOperation->SlotNumber, ItemDragDropOperation->SlotType, SlotNumber, ItemSlotType);

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag Success"));

		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Drag failed"));
		return false;
	}
}

#pragma endregion
