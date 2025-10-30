// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeltaPGameMode.h"
#include "DeltaPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeltaPGameMode::ADeltaPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
