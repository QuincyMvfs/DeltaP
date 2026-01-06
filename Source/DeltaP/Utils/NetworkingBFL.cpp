// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingBFL.h"

bool UNetworkingBFL::CheckIfLocallyOwned(const AActor* ActorToCheck)
{
	if (!ActorToCheck)
	{
		return false;
	}
	return ActorToCheck->HasLocalNetOwner();
}
