// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageFloatingText.generated.h"

UCLASS()
class EDEN_API ADamageFloatingText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageFloatingText();

	void Init(float InDamage, FColor InColor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class UTextRenderComponent* TextRender;

	float LifeTime = 1.5f;
	float ElapsedTime = 0.0f;
	float RiseSpeed = 50.0f;
};
