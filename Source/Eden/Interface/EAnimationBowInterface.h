// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EAnimationBowInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEAnimationBowInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EDEN_API IEAnimationBowInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ShootArrow() = 0;

	virtual void ShootHomingArrow(APawn* /*NearestTargetPawn*/) = 0;

	virtual void ExecuteBothSkill() = 0;

	virtual void ExecuteOneSkill() = 0;
};
