// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchingComponent.h"

#include "DeltaP/DeltaPCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCrouchingComponent::UCrouchingComponent()
{
	
}


// Called when the game starts
void UCrouchingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		Server_GetReferenceVariables();
	}
}

void UCrouchingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCrouchingComponent, IsCrouching);
}

void UCrouchingComponent::SetCrouching(bool Value)
{
	Server_SetCrouching(Value);
}

void UCrouchingComponent::Server_SetCrouching_Implementation(bool Value)
{
	IsCrouching = Value;
	
	if (Value)
	{
		if (PlayerMovementComponentRef)
		{
			UpdateVariablesToCrouching();
		}
	}
	else
	{
		if (PlayerRef->CurrentMovementState == EMovementStates::Crouching)
		{
			PlayerRef->ChangeMovementState(EMovementStates::Idle);
			Multi_SetWalkSpeed(DefaultWalkSpeed);
		}
	}
}

void UCrouchingComponent::Multi_SetWalkSpeed_Implementation(float NewSpeed)
{
	if (PlayerMovementComponentRef)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = NewSpeed;
	}
}

void UCrouchingComponent::GetReferenceVariables()
{
	Server_GetReferenceVariables();
}

void UCrouchingComponent::Server_GetReferenceVariables_Implementation()
{
	Multi_GetReferenceVariables();
}

void UCrouchingComponent::Multi_GetReferenceVariables_Implementation()
{
	PlayerRef = Cast<ADeltaPCharacter>(GetOwner());
	PlayerMovementComponentRef = PlayerRef->GetCharacterMovement();
	DefaultWalkSpeed = PlayerMovementComponentRef->GetMaxSpeed();
}

void UCrouchingComponent::UpdateVariablesToCrouching()
{
	if (PlayerRef->CurrentMovementState != EMovementStates::Sprinting)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = CrouchingSpeed;
		PlayerRef->ChangeMovementState(EMovementStates::Crouching);
		Multi_SetWalkSpeed(CrouchingSpeed);
	}
}
