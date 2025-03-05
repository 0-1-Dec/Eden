// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EPlayerController.h"

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이 모드 진입시 마우스 게임창 내부로 넣는 코드
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
