// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterNonPlayer.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "ERandomSpawn.generated.h"

UCLASS()
class EDEN_API AERandomSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AERandomSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AoE)
	USphereComponent* CollisionComponent;

public:
	UPROPERTY(EditAnywhere)
	ANavMeshBoundsVolume* NavMeshBoundsVolume;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AECharacterNonPlayer> MonsterClass;

	UPROPERTY(EditAnywhere)
	float FixedZ = 96.0f;

	UPROPERTY(EditAnywhere)
	int32 SpawnCount = 0.f;

};
