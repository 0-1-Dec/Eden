// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GeneralTurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UBTTask_GeneralTurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GeneralTurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
