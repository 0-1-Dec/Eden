// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EArrow.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DamageFloatingText.h"

// Sets default values
AEArrow::AEArrow()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Cast<UBoxComponent>(CollisionComponent)->SetBoxExtent(FVector(50,3,3));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("EProjectile"));
	SetRootComponent(CollisionComponent);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(CollisionComponent);
	ArrowMesh->SetRelativeRotation(FRotator(0, 180.f, 0));
	ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->InitialSpeed = 4000.f;
	ProjectileMovementComponent->MaxSpeed = 4000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.3f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Eden/Item/arrow.arrow'"));
	if (ArrowMeshRef.Object != nullptr)
	{
		ArrowMesh->SetStaticMesh(ArrowMeshRef.Object);
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &AEArrow::OnHit);
}

void AEArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AEArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherComp && OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
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


