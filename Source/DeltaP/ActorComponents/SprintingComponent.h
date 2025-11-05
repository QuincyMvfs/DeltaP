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

	UPROPERTY()
	float DefaultCameraFOV;

	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category="Sprinting")
	bool IsSprinting = false;
	
	UFUNCTION(BlueprintCallable, Category="Sprinting")
	void SetSprinting(bool IsRunning);

	UFUNCTION(Server, UnReliable, Category="Sprinting|Server")
	void Server_SetSprinting(bool Value);

	UFUNCTION(NetMulticast, UnReliable, Category="Sprinting|Multi")
	void Multi_SetMaxWalkSpeed(float NewSpeed, float NewFOV);

	// Get Referencing Variables
	UFUNCTION(Category="Sprinting")
	void GetReferenceVariables();
	
	UFUNCTION(Server, UnReliable, Category="Sprinting|Server")
	void Server_GetReferenceVariables();
	
	UFUNCTION(NetMulticast, UnReliable, Category="Sprinting|Multi")
	void Multi_GetReferenceVariables();
	
	void SetSprintingVariables();

	// FOV Adjusting
	UPROPERTY()
	FTimerHandle FOVTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sprinting|FOV")
	float SprintingFOV = 110.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sprinting|FOV")
	float FOVChangeMultiplier = 50.0f;

	void SetFOVTimer(const float NewFOV);
	
	void SetFOV(const float NewFOV);
	//

	// Referenced Variables
	UPROPERTY()
	UCharacterMovementComponent* PlayerMovementComponentRef;

	UPROPERTY()
	ADeltaPCharacter* PlayerRef;
	
};
