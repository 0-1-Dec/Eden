// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_GeneralAttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UBTDecorator_GeneralAttackInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_GeneralAttackInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
