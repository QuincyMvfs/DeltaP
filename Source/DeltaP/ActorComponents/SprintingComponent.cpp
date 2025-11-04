// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintingComponent.h"

#include "DeltaP/DeltaPCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USprintingComponent::USprintingComponent()
{
	
}

void USprintingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USprintingComponent, IsSprinting);
}


// Called when the game starts
void USprintingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		Server_GetReferenceVariables();
	}
}

void USprintingComponent::SetSprinting(bool IsRunning)
{
	Server_SetSprinting(IsRunning);
}

void USprintingComponent::Server_SetSprinting_Implementation(bool Value)
{
	IsSprinting = Value;

	if (Value)
	{
		if (PlayerMovementComponentRef)
		{
			SetSprintingVariables();
		}
	}
	else
	{
		if (PlayerMovementComponentRef)
		{
			if (PlayerRef->CurrentMovementState == EMovementStates::Sprinting)
			{
				PlayerRef->ChangeMovementState(EMovementStates::Idle);
				Multi_SetMaxWalkSpeed(DefaultWalkSpeed);
			}
		}
	}
}

void USprintingComponent::Multi_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	if (PlayerMovementComponentRef)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = NewSpeed;
	}
}

void USprintingComponent::GetReferenceVariables()
{
	Server_GetReferenceVariables();
}

void USprintingComponent::Server_GetReferenceVariables_Implementation()
{
	Multi_GetReferenceVariables();
}

void USprintingComponent::Multi_GetReferenceVariables_Implementation()
{
	PlayerRef = Cast<ADeltaPCharacter>(GetOwner());
	PlayerMovementComponentRef = PlayerRef->GetCharacterMovement();
	DefaultWalkSpeed = PlayerMovementComponentRef->GetMaxSpeed();
}

void USprintingComponent::SetSprintingVariables()
{
	if (PlayerRef->CurrentMovementState != EMovementStates::Crouching)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = SprintingSpeed;
		PlayerRef->ChangeMovementState(EMovementStates::Sprinting);
		Multi_SetMaxWalkSpeed(SprintingSpeed);
	}
}

