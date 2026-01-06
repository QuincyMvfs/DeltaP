// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkingBFL.generated.h"

/**
 * 
 */
UCLASS()
class DELTAP_API UNetworkingBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static bool CheckIfLocallyOwned(const AActor* ActorToCheck);
};
