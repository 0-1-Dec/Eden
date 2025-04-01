// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API AEBossAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEBossAIController();

	void RunBossAI();
	void StopBossAI();

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBBossAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTBossAsset;
	
};
