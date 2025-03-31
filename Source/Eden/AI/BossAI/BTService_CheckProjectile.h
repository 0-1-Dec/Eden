// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckProjectile.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UBTService_CheckProjectile : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckProjectile();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Casting)
	float ProjectileChance = 0.3f;
};
