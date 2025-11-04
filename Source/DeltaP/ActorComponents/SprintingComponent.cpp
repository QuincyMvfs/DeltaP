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
		if (PlayerMovementComponent)
		{
			PlayerMovementComponent->MaxWalkSpeed = SprintingSpeed;
		}
		else
		{
			PlayerMovementComponent = Cast<ADeltaPCharacter>(GetOwner())->GetCharacterMovement();
			DefaultWalkSpeed = PlayerMovementComponent->GetMaxSpeed();
			Multi_SetMaxWalkSpeed(SprintingSpeed);
		}
	}
	else
	{
		if (PlayerMovementComponent)
		{
			Multi_SetMaxWalkSpeed(DefaultWalkSpeed);
		}
	}
}

void USprintingComponent::Multi_SetMaxWalkSpeed_Implementation(float NewSpeed)
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

