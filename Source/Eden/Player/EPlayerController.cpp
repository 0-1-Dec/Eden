// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EPlayerController.h"

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
