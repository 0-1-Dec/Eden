// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "ECharacterInteractive.generated.h"

UCLASS()
class EDEN_API AECharacterInteractive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AECharacterInteractive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="NPC")
	float SenseRadius = 1000.f;

	// 외곽선이 생기는 더 가까운 반경
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="NPC")
	float OutlineRadius = 300.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="NPC")
	USkeletalMeshComponent* MeshComp;

	// 외곽선 상태 저장
	bool bOutlineOn = false;

	//플레이어에게 반환할 위젯 블루프린트
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	UUserWidget* InteractWidgetBlueprint;
};
