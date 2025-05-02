// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageFloatingText.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ADamageFloatingText::ADamageFloatingText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	RootComponent = TextRender;

	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetTextRenderColor(FColor::Red);
	TextRender->SetWorldSize(50.f);
}

// Called when the game starts or when spawned
void ADamageFloatingText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageFloatingText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 떠오르기
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += RiseSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    // 플레이어 또는 카메라 방향으로 회전
    if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FVector CameraLoc;
        FRotator CameraRot;
        PC->GetPlayerViewPoint(CameraLoc,CameraRot);

        FVector LookDir = CameraLoc - GetActorLocation();
        FRotator LookRot = FRotationMatrix::MakeFromX(LookDir).Rotator();

        SetActorRotation(FRotator(0.f,LookRot.Yaw,0.f)); // Yaw만 적용 (수평 회전만)
    }

    // 시간 체크 → 제거
    ElapsedTime += DeltaTime;
    if(ElapsedTime >= LifeTime)
    {
        Destroy();
    }
}

void ADamageFloatingText::Init(float InDamage, FColor InColor)
{
	FString Text = FString::Printf(TEXT("-%d"),FMath::RoundToInt(InDamage));
	TextRender->SetTextRenderColor(InColor);
	TextRender->SetText(FText::FromString(Text));
}