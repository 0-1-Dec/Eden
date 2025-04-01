// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTService_CheckProjectile.h"

#include "AI/EAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckProjectile::UBTService_CheckProjectile()
{
	NodeName = TEXT("Check Projectile");
	Interval = 1.0f;
	RandomDeviation = 0.1f;
}

void UBTService_CheckProjectile::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	if (FMath::FRand() <= ProjectileChance)
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if (BlackboardComponent && !BlackboardComponent->GetValueAsBool(BBKEY_ISPROJECTILE))
		{
			BlackboardComponent->SetValueAsBool(BBKEY_ISPROJECTILE, true);
		}
	}
}
