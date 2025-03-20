// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterBase.h"
#include "ECharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API AECharacterBoss : public AECharacterBase
{
	GENERATED_BODY()

public:
	AECharacterBoss();

protected:
	void SetDead() override;
};
