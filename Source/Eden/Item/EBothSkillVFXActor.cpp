// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EBothSkillVFXActor.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DamageFloatingText.h"

// Sets default values
AEBothSkillVFXActor::AEBothSkillVFXActor()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(300.f);
	CollisionComponent->SetCollisionProfileName(TEXT("EAoE"));

	RootComponent = CollisionComponent;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComponent->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void AEBothSkillVFXActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.5f);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEBothSkillVFXActor::OnSphereOverlap);
}

void AEBothSkillVFXActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* OverlappedPawn = Cast<APawn>(OtherActor))
	{
		FTimerDelegate Del;
		Del.BindLambda([this, OverlappedPawn]()
		{
			UGameplayStatics::ApplyDamage(OverlappedPawn, Damage, OverlappedPawn->GetController(), this, nullptr);
			GetWorld()->SpawnActor<ADamageFloatingText>(ADamageFloatingText::StaticClass(),OverlappedPawn->GetActorLocation() + FVector(0, 0, 100.0f),FRotator::ZeroRotator)->Init(Damage, FColor::White);
		});
		
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, Del, 0.5f, false);
	}
}
