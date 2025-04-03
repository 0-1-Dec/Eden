// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EPlayerController.h"
#include "UI/EHUDWidget.h"

AEPlayerController::AEPlayerController()
{
	static ConstructorHelpers::FClassFinder<UEHUDWidget> EHUDWidgetRef(TEXT("/Game/Eden/UI/WBP_EHUDWidget.WBP_EHUDWidget_C"));
	if (EHUDWidgetRef.Class)
	{
		EHUDWidgetClass = EHUDWidgetRef.Class;
	}
}

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이 모드 진입시 마우스 게임창 내부로 넣는 코드
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	EHUDWidget = CreateWidget<UEHUDWidget>(this, EHUDWidgetClass);
	if (EHUDWidget)
	{
		EHUDWidget->AddToViewport();
	}
}
