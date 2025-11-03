// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Trigger.generated.h"

UINTERFACE(BlueprintType)
class UTrigger : public UInterface
{
	GENERATED_BODY()
};

class DELTAP_API ITrigger
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger Events")
	void Trigger();
};
