// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShootArrow.h"
#include "Interface/EAnimationBowInterface.h"

void UAnimNotify_ShootArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IEAnimationBowInterface* AttackPawn = Cast<IEAnimationBowInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->ShootArrow();
		}
	}
}
