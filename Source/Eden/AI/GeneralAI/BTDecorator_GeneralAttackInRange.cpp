// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralAI/BTDecorator_GeneralAttackInRange.h"
#include "AI/EAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/EGeneralAIInterface.h"

UBTDecorator_GeneralAttackInRange::UBTDecorator_GeneralAttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_GeneralAttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp,NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return false;
	}

	IEGeneralAIInterface* AIPawn = Cast<IEGeneralAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}
