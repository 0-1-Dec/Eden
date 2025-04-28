// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTService_StaggerCheck.h"

#include "AIController.h"
#include "AI/EAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ECharacterBoss.h"

UBTService_StaggerCheck::UBTService_StaggerCheck()
{
	NodeName = TEXT("StaggerCheck");

	Interval = 2.0f;

	NextThreshold = 90.f;
}

void UBTService_StaggerCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return;
	}

	AECharacterBoss* Boss = Cast<AECharacterBoss>(AICon->GetPawn());
	if (!Boss)
	{
		return;
	}
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return;
	}

	// 2) 보스 HP 가져오기 (절댓값인지 퍼센트인지 프로젝트 설계에 따라 다름)
	const float BossHP = BBComp->GetValueAsFloat(BBKEY_BOSSHP);
	
	if (NextThreshold > 0.0f && BossHP <= NextThreshold)
	{
		BBComp->SetValueAsBool(BBKEY_ISSTAGGERED, true);
		Boss->bIsStaggerInProgress = true;

		NextThreshold -= 20.f;
	}
}
