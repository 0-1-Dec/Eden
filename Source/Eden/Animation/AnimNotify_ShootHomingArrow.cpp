// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShootHomingArrow.h"

#include "Character/ECharacterPlayer.h"

void UAnimNotify_ShootHomingArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AECharacterPlayer* Character = Cast<AECharacterPlayer>(MeshComp->GetOwner());
		if (IEAnimationBowInterface* AttackPawn = Cast<IEAnimationBowInterface>(MeshComp->GetOwner()))
		{
			AttackPawn->ShootHomingArrow(Character->NearestTargetPawn);
		}
	}
}
