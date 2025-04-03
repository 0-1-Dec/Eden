// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_LoopHold.h"

#include "Interface/EAnimationBowInterface.h"

void UAnimNotify_LoopHold::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,Animation,EventReference);

	if (MeshComp)
	{
		if (IEAnimationBowInterface* AttackPawn = Cast<IEAnimationBowInterface>(MeshComp->GetOwner()))
		{
			AttackPawn->LoopHold();
		}
	}
}
