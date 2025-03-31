// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponentPool.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EAoEActor.generated.h"

UCLASS()
class EDEN_API AEAoEActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEAoEActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AoE)
	float LifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AoE)
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AoE)
	USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AoE)
	UNiagaraComponent* NiagaraComponent;

	TSet<APawn*> OverlappingPawns;

	FTimerHandle DamageTimerHandle;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void ApplyContinuousDamage();
};
