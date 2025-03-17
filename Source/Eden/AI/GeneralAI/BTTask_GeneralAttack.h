// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GeneralAttack.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UBTTask_GeneralAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GeneralAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
