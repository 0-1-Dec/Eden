// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ECharacterInteractive.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ECharacterPlayer.h"

// Sets default values
AECharacterInteractive::AECharacterInteractive()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AECharacterInteractive::BeginPlay()
{
	Super::BeginPlay();

    MeshComp->SetRenderCustomDepth(false);
    MeshComp->SetCustomDepthStencilValue(1); // 외곽선 머티리얼에서 사용하는 값
    bOutlineOn = false;
}

// Called every frame
void AECharacterInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector MyLocation = GetActorLocation();
    ACharacter* ClosestPlayer = nullptr;
    AECharacterPlayer* Player = nullptr;
    float MinDistSq = SenseRadius * SenseRadius;
    float ClosestDistSq = FLT_MAX;

    // 모든 플레이어 순회
    for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if(PC && PC->GetPawn())
        {
            ACharacter* PlayerChar = Cast<ACharacter>(PC->GetPawn());
            if(PlayerChar)
            {
                float DistSq = FVector::DistSquared(MyLocation,PlayerChar->GetActorLocation());
                if(DistSq < MinDistSq)
                {
                    MinDistSq = DistSq;
                    ClosestPlayer = PlayerChar;
                    ClosestDistSq = DistSq;
                    Player = Cast<AECharacterPlayer>(ClosestPlayer);
                }
            }
        }
    }

    // 가까운 플레이어가 있으면 몸을 돌림
    if(ClosestPlayer)
    {
        FVector ToPlayer = ClosestPlayer->GetActorLocation() - MyLocation;
        FRotator TargetRot = ToPlayer.Rotation();
        TargetRot.Pitch = 0.f;
        TargetRot.Roll = 0.f;

        FRotator NewRot = FMath::RInterpTo(GetActorRotation(),TargetRot,DeltaTime,5.f);
        SetActorRotation(NewRot);

        // 외곽선 반경 이내면 외곽선 켜기
        if(ClosestDistSq < OutlineRadius * OutlineRadius)
        {
            if(!bOutlineOn)
            {
                MeshComp->SetRenderCustomDepth(true);
                MeshComp->SetCustomDepthStencilValue(1);
                bOutlineOn = true;
                if(Player) Player->CurrentInteractableActor = this;
            }
        } else
        {
            if(bOutlineOn)
            {
                if(Player){
                    Player->CurrentInteractableActor = nullptr;
                    if(Player->bInteractionUIOpen) Player->UseInteraction();
                }
                MeshComp->SetRenderCustomDepth(false);
                bOutlineOn = false;
            }
        }
    } else
    {
        Player = nullptr;
        // 플레이어가 없으면 외곽선 꺼짐
        if(bOutlineOn)
        {
            MeshComp->SetRenderCustomDepth(false);
            bOutlineOn = false;
        }
    }
}