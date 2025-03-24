// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTTask_StaggerPattern.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ECharacterBoss.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_StaggerPattern::UBTTask_StaggerPattern()
{
	NodeName = TEXT("StaggerPattern");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StaggerPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp,NodeMemory);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return EBTNodeResult::Failed;
	}

	AECharacterBoss* Boss = Cast<AECharacterBoss>(AICon->GetPawn());
	if (!Boss)
	{
		return EBTNodeResult::Failed;
	}

	UECharacterStatComponent* StatComp = Boss->FindComponentByClass<UECharacterStatComponent>();
	if (!StatComp)
	{
		return EBTNodeResult::Failed;
	}
	
	ElapsedTime = 0.f;

	bIsTaskActive = true;

	Boss->GetCharacterMovement()->SetMovementMode(MOVE_None);

	if (StaggerMontage)
	{
		Boss->PlayAnimMontage(StaggerMontage, 1.0f);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_StaggerPattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsTaskActive)
	{
		return;
	}

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AECharacterBoss* Boss = Cast<AECharacterBoss>(AICon->GetPawn());
	if (!Boss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ElapsedTime += DeltaSeconds;

	if (Boss->CurrentStaggerGauge >= MaxStaggerGauge)
	{
		Boss->bIsStaggerInProgress = false;
		bIsTaskActive = false;
		Boss->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
		UE_LOG(LogTemp, Warning, TEXT("End"));
		Boss->CurrentStaggerGauge = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	if (ElapsedTime >= StaggerDuration)
	{
		Boss->bIsStaggerInProgress = false;
		bIsTaskActive = false;
		UE_LOG(LogTemp, Warning, TEXT("Fail End"));
		Boss->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
		Boss->CurrentStaggerGauge = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	Super::TickTask(OwnerComp,NodeMemory,DeltaSeconds);
}
