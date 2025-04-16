// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatSnapshot.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Engine/GameInstance.h"
#include "Inventory/EInventoryComponent.h"
#include "Inventory/EInventorySlot.h"
#include "EGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEGameInstance();

	UPROPERTY(BlueprintReadWrite)
	FCharacterStatSnapshot StatSnapshot;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEInventorySlot> InventorySnapshot;

	UFUNCTION(BlueprintCallable)
	void CacheFromComponents(UECharacterStatComponent* StatComp, UEInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable)
	void ApplyToComponents(UECharacterStatComponent* StatComp, UEInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentStat(const FString& SlotName = TEXT("PlayerSlot"), int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable)
	bool LoadSavedState(const FString& SlotName = TEXT("PlayerSlot"), int32 UserIndex = 0);
};
