// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterBoss.h"
#include "AI/BossAI/EBossAIController.h"

AECharacterBoss::AECharacterBoss()
{
	AIControllerClass = AEBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AECharacterBoss::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
