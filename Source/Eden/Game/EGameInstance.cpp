// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EGameInstance.h"

#include "ESaveGame.h"
#include "Kismet/GameplayStatics.h"

UEGameInstance::UEGameInstance()
{
}

void UEGameInstance::CacheFromComponents(UECharacterStatComponent* StatComp, UEInventoryComponent* InventoryComp)
{
	if (StatComp)
	{
		StatSnapshot = StatComp->MakeStatSnapshot();
	}
	
	if (InventoryComp)
	{
		InventorySnapshot = InventoryComp->GetAllSlots();
	}
}

void UEGameInstance::ApplyToComponents(UECharacterStatComponent* StatComp, UEInventoryComponent* InventoryComp)
{
	if (StatComp)
	{
		StatComp->ApplyStatSnapshot(StatSnapshot);
	}

	if (InventoryComp)
	{
		if (InventorySnapshot.Num() <= 0)
		{
			InventoryComp->InitializeInventory(25);
		}
		else
		{
			InventoryComp->InitFromArray(InventorySnapshot);	
		}
	}
}

void UEGameInstance::SaveCurrentStat(const FString& SlotName, int32 UserIndex)
{
	UESaveGame* SaveObj =
		Cast<UESaveGame>(UGameplayStatics::CreateSaveGameObject(UESaveGame::StaticClass()));

	SaveObj->StatSnapshot      = StatSnapshot;
	SaveObj->InventorySnapshot = InventorySnapshot;

	UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);
}

bool UEGameInstance::LoadSavedState(const FString& SlotName, int32 UserIndex)
{
	if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
	{
		if (auto* SG = Cast<UESaveGame>(Loaded))
		{
			StatSnapshot      = SG->StatSnapshot;
			// InventorySnapshot = SG->InventorySnapshot;
			return true;
		}
	}
	return false;
}
