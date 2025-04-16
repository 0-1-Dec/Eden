// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/LevelTransferVolume.h"

#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/BoxComponent.h"
#include "Game/EGameInstance.h"
#include "Inventory/EInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelTransferVolume::ALevelTransferVolume()
{
 	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelTransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ALevelTransferVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelTransferVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn || !Pawn->IsPlayerControlled()) return;

	UECharacterStatComponent* StatComp = Pawn->FindComponentByClass<UECharacterStatComponent>();
	UEInventoryComponent* InventoryComp = Pawn->FindComponentByClass<UEInventoryComponent>();

	if (!StatComp || !InventoryComp) return;

	if (UEGameInstance* GI = GetGameInstance<UEGameInstance>())
	{
		GI->CacheFromComponents(StatComp, InventoryComp);
	}

	if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}


