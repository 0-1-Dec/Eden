// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPlayerController();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UEHUDWidget> EHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UEHUDWidget> EHUDWidget;
};
