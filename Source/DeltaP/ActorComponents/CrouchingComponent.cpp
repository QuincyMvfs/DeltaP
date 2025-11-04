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
		if (PlayerMovementComponent)
		{
			PlayerMovementComponent->MaxWalkSpeed = CrouchingSpeed;
		}
		else
		{
			PlayerMovementComponent = Cast<ADeltaPCharacter>(GetOwner())->GetCharacterMovement();
			DefaultWalkSpeed = PlayerMovementComponent->GetMaxSpeed();
			Multi_SetWalkSpeed(CrouchingSpeed);
		}
	}
	else
	{
		if (PlayerMovementComponent)
		{
			Multi_SetWalkSpeed(DefaultWalkSpeed);
		}
	}
}

void UCrouchingComponent::Multi_SetWalkSpeed_Implementation(float NewSpeed)
{
	if (PlayerMovementComponent)
	{
		PlayerMovementComponent->MaxWalkSpeed = NewSpeed;
	}
	else
	{
		PlayerMovementComponent = Cast<ADeltaPCharacter>(GetOwner())->GetCharacterMovement();
		PlayerMovementComponent->MaxWalkSpeed = NewSpeed;
	}
}
