// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralAI/BTTask_GeneralAttack.h"
#include "AIController.h"
#include "Interface/EGeneralAIInterface.h"

UBTTask_GeneralAttack::UBTTask_GeneralAttack()
{
	
}

EBTNodeResult::Type UBTTask_GeneralAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IEGeneralAIInterface* AIPawn = Cast<IEGeneralAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FGeneralAIAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}
