// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/BTTask_CastingBreath.h"

#include "AIController.h"
#include "MaterialHLSLTree.h"
#include "AI/EAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ECharacterBoss.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_CastingBreath::UBTTask_CastingBreath()
{
	NodeName = TEXT("Casting Breath");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CastingBreath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp,NodeMemory);

	ElapsedTime = 0.f;
	bDidBreath = false;
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AECharacterBoss* Boss = Cast<AECharacterBoss>(AIController->GetPawn());
	if (!Boss)
	{
		return EBTNodeResult::Failed;
	}

	Boss->GetCharacterMovement()->SetMovementMode(MOVE_None);

	if (CastingMontage)
	{
		if (UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(CastingMontage, 1.0f);
		}
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_CastingBreath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp,NodeMemory,DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	APawn* Boss = AIController->GetPawn();
	if (!Boss)
	{
		return;
	}

	AECharacterBoss* BossCharcter = Cast<AECharacterBoss>(AIController->GetPawn());
	if (!BossCharcter)
	{
		return;
	}

	if (ElapsedTime < CastTime)
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			FRotator CurrentRotation = BossCharcter->GetActorRotation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(BossCharcter->GetActorLocation(), PlayerPawn->GetActorLocation());
			TargetRotation.Pitch = 0;
			TargetRotation.Roll = 0;
			const float RotateSpeed = 180.f;
			FRotator NewRot = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaSeconds, RotateSpeed);
			BossCharcter->SetActorRotation(NewRot);
		}
		return;
	}

	if (!bDidBreath)
	{
		bDidBreath = true;

		if (BreathMontage)
		{
			if (UAnimInstance* AnimInst = BossCharcter->GetMesh()->GetAnimInstance())
			{
				AnimInst->Montage_Play(BreathMontage, 1.0f);
			}
		}
	}

	if (BreathVfxSystem && !bHasSpawnedVFX)
	{
		bHasSpawnedVFX = true;

		const FVector  SocketLoc = BossCharcter->GetMesh()->GetSocketLocation(TEXT("MouthSocket"));
		const FRotator ActorRot = BossCharcter->GetActorRotation();

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			BreathVfxSystem,
			BossCharcter->GetMesh(),
			TEXT("MouthSocket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true);
	}

	float TimeSinceBreathStart = ElapsedTime - CastTime;
	if (TimeSinceBreathStart <= BreathTime)
	{
		DamageAccum += DeltaSeconds;

		if (DamageAccum >= DamageInterval)
		{
			const FVector BreathOrigin = BossCharcter->GetMesh()->GetSocketLocation(TEXT("MouthSocket"));
			const FVector ForwardDir = BossCharcter->GetActorForwardVector();

#if ENABLE_DRAW_DEBUG
			DrawDebugCone(
				Boss->GetWorld(),
				BreathOrigin,
				ForwardDir,
				Range,
				FMath::DegreesToRadians(Angle),
				FMath::DegreesToRadians(Angle),
				12,
				FColor::Red,
				false,
				0.1f  // Tick마다 재그리면 0.1초면 충분
			);
#endif
			
			TArray<AActor*> AllTargets;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllTargets);

			for (AActor* Target : AllTargets)
			{
				if (Target == BossCharcter)
				{
					continue;
				}

				FVector ToTarget = Target->GetActorLocation() - Boss->GetActorLocation();
				float Distance = ToTarget.Size();
				ToTarget.Normalize();

				float Dot = FVector::DotProduct(BossCharcter->GetActorForwardVector(), ToTarget);
				float Degree = FMath::RadiansToDegrees(FMath::Acos(Dot));

				if (Distance <= Range && Degree <= Angle)
				{
					UE_LOG(LogTemp, Warning, TEXT("Breath Applied"));
					UGameplayStatics::ApplyDamage(Target, 20.f, AIController, BossCharcter, nullptr);
				}
				
			}

			DamageAccum = 0.f;
		}
		// 아직 브레스 끝나지 않았으므로 그대로 진행
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISCASTING, false);

	BossCharcter->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);

	bHasSpawnedVFX = false;
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
