// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "ESkillPathVFX.generated.h"

UCLASS()
class EDEN_API AESkillPathVFX : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AESkillPathVFX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=VFX)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=VFX)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	TSubclassOf<class AEBothSkillVFXActor> LastSkillVFX;

private:
	float Elapsed = 0.f;

	bool bIsSpawned = false;
};
