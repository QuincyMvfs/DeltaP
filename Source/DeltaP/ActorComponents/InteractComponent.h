// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeltaP/Interfaces/Interact.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessfulHit, FInteractionInfo, InteractionInfo);


UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class DELTAP_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TryUpdatingReferences();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void TryInteract();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void InteractComplete();
	
	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	AActor* OwningActor;

	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	APlayerCameraManager* CameraManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float InteractDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|Trace Settings")
	float CapsuleRadius = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|Trace Settings")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|Trace Settings", meta=(EditCondition="DrawDebugType == EDrawDebugTrace::ForDuration") )
	float DebugTraceDuration = 0.0f;

	UPROPERTY(BlueprintAssignable, Category = "Interact")
	FOnSuccessfulHit OnSuccessfulHit;
};
