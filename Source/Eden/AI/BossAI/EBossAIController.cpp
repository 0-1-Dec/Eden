// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/EBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEBossAIController::AEBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Eden/AI/Boss/BB_EBossCharacter.BB_EBossCharacter'"));
	if (BBAssetRef.Object)
	{
		BBBossAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Eden/AI/Boss/BT_EGBossCharacter.BT_EGBossCharacter'"));
	if (BTAssetRef.Object)
	{
		BTBossAsset = BTAssetRef.Object;
	}
}

void AEBossAIController::RunBossAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBBossAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTBossAsset);
		ensure(RunResult);
	}
}

void AEBossAIController::StopBossAI()
{
	if (UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BTComponent->StopTree();
	}
}
