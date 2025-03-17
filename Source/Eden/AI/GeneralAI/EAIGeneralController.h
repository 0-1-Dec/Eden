// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EAIGeneralController.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API AEAIGeneralController : public AAIController
{
	GENERATED_BODY()

public:
	AEAIGeneralController();

	void RunGeneralAI();
	void StopGeneralAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBGeneralAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTGeneralAsset;
	
};
