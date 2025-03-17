// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EInventorySlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/EInventoryDragDropOperation.h"

#include <comdef.h>

#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Character/ECharacterPlayer.h"

void UEInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IconImage = Cast<UImage>(GetWidgetFromName("IconImage"));

	QuantityText = Cast<UTextBlock>(GetWidgetFromName("QuantityText"));
}

FReply UEInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (IsEmpty())
		{
			return FReply::Handled();
			
		}
		return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return Super::NativeOnMouseButtonDown(Geometry,MouseEvent);
}

void UEInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (IsEmpty())
	{
		return;
	}
	
	UEInventoryDragDropOperation* DragOp = NewObject<UEInventoryDragDropOperation>(this);
	DragOp->DraggedSlot = SlotData;
	DragOp->DefaultDragVisual = this;
	DragOp->SourceSlotIndex = SlotIndex;
	DragOp->Offset = InGeometry.GetLocalSize() * 0.005;
	
	OutOperation = DragOp;
}

void UEInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UEInventoryDragDropOperation* DragOp = Cast<UEInventoryDragDropOperation>(InOperation);
	if (DragOp && !IsEmpty())
	{
		if (AECharacterPlayer* Player = Cast<AECharacterPlayer>(GetOwningPlayerPawn()))
		{
			if (Player->InventoryComponent)
			{
				Player->InventoryComponent->RemoveItem(DragOp->DraggedSlot.ItemData, DragOp->SourceSlotIndex);
			}
		}
	}
	
	Super::NativeOnDragCancelled(InDragDropEvent,InOperation);
}

void UEInventorySlotWidget::SetSlotData(const FEInventorySlot& NewSlotData)
{
	SlotData = NewSlotData;
	UpdateSlotUI();
}

void UEInventorySlotWidget::UpdateSlotUI()
{
	if (SlotData.ItemData == nullptr || SlotData.Quantity <= 0)
	{
		if (IconImage)
			IconImage->SetBrushFromTexture(nullptr);
		if (QuantityText)
			QuantityText->SetText(FText::GetEmpty());
		return;
	}

	if (IconImage && SlotData.ItemData->ItemIcon)
	{
		IconImage->SetBrushFromTexture(SlotData.ItemData->ItemIcon);
	}

	if (QuantityText)
	{
		QuantityText->SetText(FText::AsNumber(SlotData.Quantity));
	}
}

void UEInventorySlotWidget::SetSlotIndex(int32 NewSlotIndex)
{
	SlotIndex = NewSlotIndex;
}

bool UEInventorySlotWidget::IsEmpty() const
{
	return (SlotData.ItemData == nullptr || SlotData.Quantity <= 0);
}

int32 UEInventorySlotWidget::GetSlotIndex() const
{
	return SlotIndex;
}

