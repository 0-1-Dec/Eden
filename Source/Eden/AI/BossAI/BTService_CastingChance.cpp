// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTService_CastingChance.h"

#include "AI/EAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CastingChance::UBTService_CastingChance()
{
	NodeName = TEXT("Casting Chance");
	Interval = 1.0f;
	RandomDeviation = 0.1f;
}

void UBTService_CastingChance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	if (FMath::FRand() <= TriggerChance)
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if (BlackboardComponent && !BlackboardComponent->GetValueAsBool(BBKEY_ISCASTING))
		{
			BlackboardComponent->SetValueAsBool(BBKEY_ISCASTING, true);
		}
	}
}
