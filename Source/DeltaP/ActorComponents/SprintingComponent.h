// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeltaP/DeltaPCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SprintingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTAP_API USprintingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USprintingComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintingSpeed = 1000.0f;

	UPROPERTY()
	float DefaultWalkSpeed;

	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category="Sprinting")
	bool IsSprinting = false;
	
	UFUNCTION(BlueprintCallable, Category="Sprinting")
	void SetSprinting(bool IsRunning);

	UFUNCTION(Server, UnReliable, Category="Sprinting|Server")
	void Server_SetSprinting(bool Value);

	UFUNCTION(NetMulticast, UnReliable, Category="Sprinting|Multi")
	void Multi_SetMaxWalkSpeed(float NewSpeed);

	// Get Referencing Variables
	UFUNCTION(Category="Sprinting")
	void GetReferenceVariables();
	
	UFUNCTION(Server, UnReliable, Category="Sprinting|Server")
	void Server_GetReferenceVariables();
	
	UFUNCTION(NetMulticast, UnReliable, Category="Sprinting|Multi")
	void Multi_GetReferenceVariables();
	
	void SetSprintingVariables();
	
	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponentRef;

	UPROPERTY()
	ADeltaPCharacter* PlayerRef;
};
