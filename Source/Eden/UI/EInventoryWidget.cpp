// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/EInventoryWidget.h"
#include "EInventorySlotWidget.h"
#include "Character/ECharacterPlayer.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Inventory/EInventoryDragDropOperation.h"

void UEInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!LinkedInventory)
    {
        if (APlayerController* PC = GetOwningPlayer())
        {
            if (AECharacterPlayer* Character = Cast<AECharacterPlayer>(PC->GetPawn()))
            {
                LinkedInventory = Character->InventoryComponent;
            }
        }
    }

    CreateInventorySlots();
    RefreshInventory();

    SetAlignmentInViewport(FVector2D(0.f, 0.f));

    if (LinkedInventory)
    {
        LinkedInventory->OnInventoryChanged.AddDynamic(this, &UEInventoryWidget::HandleInventoryChanged);
    }
}

bool UEInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (UEInventoryDragDropOperation* DragOp = Cast<UEInventoryDragDropOperation>(InOperation))
    {
        // 드롭 이벤트의 스크린 좌표
        FVector2D DropScreenPos = InDragDropEvent.GetScreenSpacePosition();

        bool bFoundSlot = false;
        UEInventorySlotWidget* TargetSlotWidget = nullptr;
        // SlotsGrid의 자식 슬롯 위젯 각각의 CachedGeometry를 통해 hit test
        if (SlotsGrid)
        {
            int32 ChildCount = SlotsGrid->GetChildrenCount();
            for (int32 i = 0; i < ChildCount; i++)
            {
                UWidget* Child = SlotsGrid->GetChildAt(i);
                if (Child && Child == DragOp->DefaultDragVisual)
                {
                    continue;
                }
                if (Child && Child->GetCachedGeometry().IsUnderLocation(DropScreenPos))
                {
                    bFoundSlot = true;
                    TargetSlotWidget = Cast<UEInventorySlotWidget>(Child);
                    break;
                }
            }
        }
        
        if (bFoundSlot)
        {
            // 영역 내이면 이동 또는 교환 처리
            int32 SourceSlotIndex = DragOp->SourceSlotIndex;
            int32 TargetSlotIndex = TargetSlotWidget->GetSlotIndex();
            if (TargetSlotWidget->IsEmpty())
            {
                if (LinkedInventory)
                {
                    LinkedInventory->MoveItem(SourceSlotIndex, TargetSlotIndex);
                }
            }
            else
            {
                if (LinkedInventory)
                {
                    LinkedInventory->SwapItems(SourceSlotIndex, TargetSlotIndex);
                }
            }
        }
        return true;
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UEInventoryWidget::CreateInventorySlots()
{
    if (!SlotsGrid || !SlotWidgetClass || !LinkedInventory)
        return;

    SlotsGrid->ClearChildren();

    int32 SlotCount = LinkedInventory->GetSlotCount();
    int32 Columns = 5;
    for (int32 i = 0; i < SlotCount; i++)
    {
        UEInventorySlotWidget* SlotWidget = CreateWidget<UEInventorySlotWidget>(GetWorld(), SlotWidgetClass, FName(*FString::Printf(TEXT("Slot %i"), i)));
        if (!SlotWidget)
            continue;

        SlotWidget->SetSlotIndex(i);

        UUniformGridSlot* GridSlot = SlotsGrid->AddChildToUniformGrid(SlotWidget);
        if (GridSlot)
        {
            GridSlot->SetRow(i / Columns);
            GridSlot->SetColumn(i % Columns);
            GridSlot->SetHorizontalAlignment(HAlign_Fill);
            GridSlot->SetVerticalAlignment(VAlign_Fill);
        }
    }
}

void UEInventoryWidget::RefreshInventory()
{
    if (!SlotsGrid || !LinkedInventory)
        return;

    int32 SlotCount = LinkedInventory->GetSlotCount();
    int32 NumChildren = SlotsGrid->GetChildrenCount();
    for (int32 i = 0; i < SlotCount && i < NumChildren; i++)
    {
        FEInventorySlot SlotData = LinkedInventory->GetSlot(i);
        UWidget* Child = SlotsGrid->GetChildAt(i);
        if (UEInventorySlotWidget* SlotWBP = Cast<UEInventorySlotWidget>(Child))
        {
            SlotWBP->SetSlotData(SlotData);
        }
    }
}

FReply UEInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && HeaderBox && HeaderBox->IsHovered())
    {
        bDragging = true;
        const FVector2D ScreenPos = InMouseEvent.GetScreenSpacePosition();
        DragOffset = InGeometry.AbsoluteToLocal(ScreenPos);
        return FReply::Handled().CaptureMouse(GetCachedWidget().ToSharedRef());
    }
    return Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
}

FReply UEInventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }
    return Super::NativeOnMouseButtonUp(InGeometry,InMouseEvent);
}

FReply UEInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDragging)
    {
        const FVector2D ScreenPos = InMouseEvent.GetScreenSpacePosition();
        const FVector2D NewPos = ScreenPos - DragOffset;
        
        SetPositionInViewport(NewPos, true); 
        
        return FReply::Handled();
    }
    return Super::NativeOnMouseMove(InGeometry,InMouseEvent);
}

void UEInventoryWidget::HandleInventoryChanged()
{
    RefreshInventory();
}
