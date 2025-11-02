// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DPBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DELTAP_API UDPBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool CheckIfLocallyOwned(const AActor* ActorToCheck);
	
};
