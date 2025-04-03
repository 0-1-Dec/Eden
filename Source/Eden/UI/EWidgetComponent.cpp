// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EWidgetComponent.h"

#include "EUserWidget.h"

void UEWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	UEUserWidget* EUserWidget = Cast<UEUserWidget>(GetWidget());
	if (EUserWidget)
	{
		EUserWidget->SetOwningActor(GetOwner());
	}
}
