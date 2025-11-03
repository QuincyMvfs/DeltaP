// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DeltaPCharacter.generated.h"

class USprintingComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADeltaPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Actor Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Components|Movement", meta = (AllowPrivateAccess = "true"))
	USprintingComponent* SprintingComponent;


public:
	ADeltaPCharacter();
	

protected:

	/** Called for movement input */
	UFUNCTION(BlueprintCallable)
	void Move(const FVector2D MovementVector);

	UFUNCTION(BlueprintCallable)
	/** Called for looking input */
	void Look(const FVector2D LookAxisVector);
			

protected:

	virtual void NotifyControllerChanged() override;
	void GenerateMovementDefaults();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

