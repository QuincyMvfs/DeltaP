// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeltaP/DeltaPCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrouchingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTAP_API UCrouchingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrouchingComponent();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// CROUCHING VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Crouching")
	float CrouchingSpeed = 300.0f;

	UPROPERTY()
	float DefaultWalkSpeed;
	
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category="Crouching")
	bool IsCrouching = false;
	//
	
	// Sets if the player is Crouching
	UFUNCTION(BlueprintCallable, Category="Crouching")
	void SetCrouching(bool Value);

	UFUNCTION(Server, UnReliable, Category="Crouching|Server")
	void Server_SetCrouching(bool Value);

	UFUNCTION(NetMulticast, UnReliable, Category="Crouching|Multi")
	void Multi_SetWalkSpeed(const float NewSpeed, const float NewCameraPosition);
	//
	
	// Gets and Sets the Referenced Variables
	UFUNCTION(Category="Crouching")
	void GetReferenceVariables();
	
	UFUNCTION(Server, UnReliable, Category="Crouching|Server")
	void Server_GetReferenceVariables();
	
	UFUNCTION(NetMulticast, UnReliable, Category="Crouching|Multi")
	void Multi_GetReferenceVariables();

	void UpdateVariablesToCrouching();
	//
	
	// CROUCHING CAMERA VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Crouching|Camera")
	float CrouchCameraZPos = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Crouching|Camera")
	float CrouchingCameraSpeed = 50.0f;
	
	UPROPERTY()
	float DefaultCameraZPos;

	UPROPERTY()
	FTimerHandle CrouchingTimer;
	
	void SetCrouchingCameraTimer(const float TargetPosition);
	
	void SetCameraPosition(const float TargetPosition);
	//

	// REFERENCED VARIABLES
	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponentRef;

	UPROPERTY()
	ADeltaPCharacter* PlayerRef;
	

		
};
