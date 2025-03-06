// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EGameMode.h"
#include "EGameMode.h"
#include "Player/EPlayerController.h"

AEGameMode::AEGameMode()
{
	// 폰 클래스 지정
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Eden/Blueprints/BP_ECharacterPlayer.BP_ECharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Eden/Blueprints/BP_ECharacterController.BP_ECharacterController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
}
