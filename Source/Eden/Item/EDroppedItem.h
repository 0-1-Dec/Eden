// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameData/EDropDataAsset.h"
#include "EDroppedItem.generated.h"

UCLASS()
class EDEN_API AEDroppedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDroppedItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Item)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Item)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Item)
	UEItemDataAsset* ItemData;

	virtual void Init(UEItemDataAsset* InItemData);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
							UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,
							bool bFromSweep,const FHitResult& SweepResult);
};
