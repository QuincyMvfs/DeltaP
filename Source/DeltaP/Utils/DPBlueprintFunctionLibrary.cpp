// Fill out your copyright notice in the Description page of Project Settings.


#include "DPBlueprintFunctionLibrary.h"

bool UDPBlueprintFunctionLibrary::CheckIfLocallyOwned(const AActor* ActorToCheck)
{
	if (!ActorToCheck)
	{
		return false;
	}
	return ActorToCheck->HasLocalNetOwner();
}
