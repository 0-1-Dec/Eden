// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/EItemDataAsset.h"
#include "EInventorySlot.h"
#include "EInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDEN_API UEInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 MaxSlots = 25;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FEInventorySlot> Slots;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeInventory(int32 InSlotCount);

	UFUNCTION(BlueprintCallable)
	bool AddItem(UEItemDataAsset* ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UEItemDataAsset* ItemData, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	FEInventorySlot GetSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SetSlotInfo(int32 SlotIndex, const FEInventorySlot& NextSlot);

	UFUNCTION(BlueprintCallable)
	void MoveItem(int32 SourceSlotIndex, int32 TargetSlotIndex);

	UFUNCTION(BlueprintCallable)
	void SwapItems(int32 SourceSlotIndex, int32 TargetSlotIndex);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSlotCount() const { return Slots.Num(); }

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryChangedDelegate OnInventoryChanged;
};
