// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ERandomSpawn.h"

#include "NavigationSystem.h"
#include "AI/GeneralAI/EAIGeneralController.h"

// Sets default values
AERandomSpawn::AERandomSpawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(800.f);
	CollisionComponent->SetCollisionProfileName(TEXT("EAoE"));
	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void AERandomSpawn::BeginPlay()
{
	Super::BeginPlay();
}

void AERandomSpawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn || !Pawn->IsPlayerControlled()) return;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FVector Origin, Extent;
	NavMeshBoundsVolume->GetActorBounds(false, Origin, Extent);
	
	for (int32 i = 0; i < SpawnCount; i++)
	{
		FNavLocation NavLoc;
		const float Radius = FMath::Max(Extent.X, Extent.Y);
		if (NavSys->GetRandomPointInNavigableRadius(Origin, Radius, NavLoc))
		{
			FVector SpawnPos = NavLoc.Location;
			SpawnPos.Z = FixedZ;

			GetWorld()->SpawnActor<AECharacterNonPlayer>(MonsterClass, SpawnPos, FRotator::ZeroRotator);
		}
	}
}


