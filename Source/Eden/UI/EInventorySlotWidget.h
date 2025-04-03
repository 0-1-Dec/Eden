// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/EInventorySlot.h"
#include "EInventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UPROPERTY()
	TObjectPtr<class UImage> IconImage;

	UPROPERTY()
	TObjectPtr<class UTextBlock> QuantityText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	UTexture2D* Icon;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FEInventorySlot SlotData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SlotIndex;

public:
	UFUNCTION(BlueprintCallable)
	void SetSlotData(const FEInventorySlot& NewSlotData);

	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int32 NewSlotIndex);

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable)
	int32 GetSlotIndex() const;

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateSlotUI();
};
