// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTAP_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	

public:	
	UAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
