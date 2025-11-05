// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EMovementStates.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DeltaPCharacter.generated.h"

class UCrouchingComponent;
class USprintingComponent;
class UCameraComponent;
class UInputMappingContext;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADeltaPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Actor Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Components|Movement", meta = (AllowPrivateAccess = "true"))
	USprintingComponent* SprintingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Components|Movement", meta = (AllowPrivateAccess = "true"))
	UCrouchingComponent* CrouchingComponent;


public:
	ADeltaPCharacter();
	

protected:

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FVector2D MovementVector);

	UFUNCTION(BlueprintCallable)
	/** Called for looking input */
	void Look(const FVector2D LookAxisVector);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void NotifyControllerChanged() override;
	void GenerateMovementDefaults();

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementStates CurrentMovementState;
	
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementStates NewState);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_ChangeMovementState(EMovementStates NewState);


public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

