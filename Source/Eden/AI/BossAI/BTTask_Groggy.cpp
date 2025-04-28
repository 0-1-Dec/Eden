// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTTask_Groggy.h"

#include "AIController.h"
#include "AI/EAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ECharacterBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Groggy::UBTTask_Groggy()
{
	NodeName = TEXT("Groggy");
	bNotifyTick = true;
	WaitTime = 5.f;
}

EBTNodeResult::Type UBTTask_Groggy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp,NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AECharacterBoss* Boss = Cast<AECharacterBoss>(ControllingPawn);
	if (!Boss)
	{
		return EBTNodeResult::Failed;
	}

	Boss->GetCharacterMovement()->SetMovementMode(MOVE_None);

	if (GroggyAnimMontage)
	{
		Boss->PlayAnimMontage(GroggyAnimMontage);
	}

	ElapsedTime = 0.f;

	return EBTNodeResult::InProgress;
}

void UBTTask_Groggy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp,NodeMemory,DeltaSeconds);

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

	if (ElapsedTime >= WaitTime)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISSTAGGERED, false);
		
		UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(RecoverAnimMontage);

		SavedOwnerComp = &OwnerComp;

		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool IsProperlyEnded)
		{
			if (AAIController* AICon = SavedOwnerComp->GetAIOwner())
			{
				if(AECharacterBoss* Boss = Cast<AECharacterBoss>(AICon->GetPawn()))
				{
					Boss->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
				}
			}

			FinishLatentTask(*SavedOwnerComp, EBTNodeResult::Succeeded);
		});
		
		AnimInstance->Montage_SetEndDelegate(EndDelegate, RecoverAnimMontage);
	}
}
