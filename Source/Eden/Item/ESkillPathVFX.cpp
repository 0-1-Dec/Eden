// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ESkillPathVFX.h"

#include "EBothSkillVFXActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AESkillPathVFX::AESkillPathVFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(50.f);

	RootComponent = CollisionComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AESkillPathVFX::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComponent->Activate(true);
}

// Called every frame
void AESkillPathVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Step = MoveSpeed * DeltaTime;
	AddActorWorldOffset(GetActorForwardVector() * Step, true);

	Elapsed += Step;

	if (Elapsed / Distance >= 0.4f)
	{
		if (!bIsSpawned)
		{
			FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 600.f;
			FRotator SpawnRotation = GetActorRotation();
		
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
	
			GetWorld()->SpawnActor<AEBothSkillVFXActor>(LastSkillVFX, SpawnLocation, SpawnRotation, SpawnParams);

			bIsSpawned = true;
		}
	}
	
	if (Elapsed >= Distance)
	{
		Destroy();
	}
}

