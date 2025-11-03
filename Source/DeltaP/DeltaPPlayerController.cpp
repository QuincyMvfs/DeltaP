// Fill out your copyright notice in the Description page of Project Settings.


#include "DeltaPPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


class UEnhancedInputLocalPlayerSubsystem;

void ADeltaPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		
	}
}

