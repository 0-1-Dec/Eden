// Fill out your copyright notice in the Description page of Project Settings.


#include "ESaveGameWidget.h"

#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/Button.h"
#include "Game/ESaveGame.h"
#include "Kismet/GameplayStatics.h"

UESaveGameWidget::UESaveGameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UESaveGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SaveButton = Cast<UButton>(GetWidgetFromName(TEXT("SaveButton")));
	ensure(SaveButton);

	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &UESaveGameWidget::OnButtonClicked);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			UECharacterStatComponent* StatComp = Pawn->FindComponentByClass<UECharacterStatComponent>();
			if (StatComp)
			{
				StatComponent = StatComp;
			}
		}
	}
}

void UESaveGameWidget::OnButtonClicked()
{
	if (StatComponent)
	{
		FCharacterStatSnapshot Snap = StatComponent->MakeStatSnapshot();

		UESaveGame* SaveGame =  Cast<UESaveGame>(
			UGameplayStatics::CreateSaveGameObject(UESaveGame::StaticClass())
		);

		if (!SaveGame)
			return;

		SaveGame->StatSnapshot = Snap;

		const FString SlotName = TEXT("PlayerSlot");
		const int32 UserIndex = 0;
		if (UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex))
		{
			UE_LOG(LogTemp, Log, TEXT("저장 성공"));
		}
	}
}
