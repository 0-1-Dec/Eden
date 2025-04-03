// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DamageFloatingText.h"

// Sets default values
AEArrow::AEArrow()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	CollisionComponent->SetSphereRadius(10.f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.3f;

	CollisionComponent->OnComponentHit.AddDynamic(this, &AEArrow::OnHit);
}

void AEArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AEArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);

		FVector SpawnLoc = Hit.GetActor()->GetActorLocation() + FVector(0,0,100);
		GetWorld()->SpawnActor<ADamageFloatingText>(ADamageFloatingText::StaticClass(),SpawnLoc,FRotator::ZeroRotator)->Init(Damage);
		
		Destroy();
	}
}

void AEArrow::InitProjectile(float InDamage, float InSpeed)
{
	this->Damage = InDamage;
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * InSpeed;

		ProjectileMovementComponent->InitialSpeed = InSpeed;
		ProjectileMovementComponent->MaxSpeed = InSpeed;
	}
}


