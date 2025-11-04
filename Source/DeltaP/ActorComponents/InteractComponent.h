// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeltaP/Interfaces/Interact.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractComponent.generated.h"

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class DELTAP_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
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

	UPROPERTY(BlueprintReadOnly, Category = "Interact")
	FTimerHandle HoldTimer;

	UPROPERTY()
	AActor* TargetActor;
	
public:	
	UInteractComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TryUpdatingReferences();

	UFUNCTION()
	FHitResult TryExecuteTrace();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void TryInteract();

	UFUNCTION(Server, Unreliable)
	void Server_InteractComplete(AActor* HitActor);

	UFUNCTION()
	void OnHoldFinished();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void CancelInteract();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnSuccessfulHit(const FInteractionInfo& Info);

	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnCancelInteract();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnInteractComplete();
};
