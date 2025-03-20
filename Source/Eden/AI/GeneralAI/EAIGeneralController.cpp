// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralAI/EAIGeneralController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EAI.h"

AEAIGeneralController::AEAIGeneralController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Eden/AI/General/BB_EGeneralCharacter.BB_EGeneralCharacter'"));
	if (BBAssetRef.Object)
	{
		BBGeneralAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Eden/AI/General/BT_EGeneralCharacter.BT_EGeneralCharacter'"));
	if (BTAssetRef.Object)
	{
		BTGeneralAsset = BTAssetRef.Object;
	}
}

void AEAIGeneralController::RunGeneralAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBGeneralAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		
		bool RunResult = RunBehaviorTree(BTGeneralAsset);
		ensure(RunResult);
	}
}

void AEAIGeneralController::StopGeneralAI()
{
	if (UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BTComponent->StopTree();
	}
}

void AEAIGeneralController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunGeneralAI();
}
