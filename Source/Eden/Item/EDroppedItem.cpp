// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EDroppedItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameData/EDropDataAsset.h"
#include "Character/ECharacterPlayer.h"
#include "Inventory/EInventoryComponent.h"

// Sets default values
AEDroppedItem::AEDroppedItem()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComponent;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,&AEDroppedItem::OnOverlapBegin);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AEDroppedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEDroppedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEDroppedItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	if(OtherActor && (OtherActor != this))
	{
		AECharacterPlayer* Player = Cast<AECharacterPlayer>(OtherActor);
		if(Player && Player->InventoryComponent)
		{
			if(bool bAdded = Player->InventoryComponent->AddItem(ItemData,1))
				Destroy();
		}
	}
}

void AEDroppedItem::Init(UEItemDataAsset* InItemData,int32 InCount)
{
	ItemData = InItemData;
	StackCount = InCount;
}
