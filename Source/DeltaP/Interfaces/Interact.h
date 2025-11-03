// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Hold = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoldTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractMessage;
};



UINTERFACE(BlueprintType, Blueprintable)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

class DELTAP_API IInteract
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact(AActor* InstigatorActor);

	UFUNCTION(BLueprintCallable, BlueprintNativeEvent, Category = "Interact")
	bool CanInteract() const;

	UFUNCTION(BLueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FInteractionInfo GetInteractInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	UAnimMontage* GetInteractMontage() const;
};
