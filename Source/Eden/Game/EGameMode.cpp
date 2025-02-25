// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EGameMode.h"
#include "EGameMode.h"
#include "Player/EPlayerController.h"

AEGameMode::AEGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_c"));
	//if (ThirdPersonClassRef.Class)
	//{
		//DefaultPawnClass = ThirdPersonClassRef.Class;
	//}
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Eden.ECharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Eden.EPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
}
