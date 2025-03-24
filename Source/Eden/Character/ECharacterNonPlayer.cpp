// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterNonPlayer.h"
#include "AI/GeneralAI/EAIGeneralController.h"
#include "CharacterStat/ECharacterStatComponent.h"

AECharacterNonPlayer::AECharacterNonPlayer()
{
	AIControllerClass = AEAIGeneralController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AECharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->SetMaxHp(100.f);
}

void AECharacterNonPlayer::SetDead()
{
	Super::SetDead();

	if (AEAIGeneralController* AEIController = Cast<AEAIGeneralController>(GetController()))
	{
		AEIController->StopGeneralAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

float AECharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AECharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AECharacterNonPlayer::GetAIAttackRange()
{
	return 140.0f;
}

float AECharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AECharacterNonPlayer::SetAIAttackDelegate(const FGeneralAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AECharacterNonPlayer::AttackByAI()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackByAI called"));
	ProcessComboCommand();
}

void AECharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
