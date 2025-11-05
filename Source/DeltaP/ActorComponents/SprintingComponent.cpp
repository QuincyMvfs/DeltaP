// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintingComponent.h"

#include "Camera/CameraComponent.h"
#include "DeltaP/DeltaPCharacter.h"
#include "Math/UnitConversion.h"
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
				SetFOV(DefaultCameraFOV);
				PlayerRef->ChangeMovementState(EMovementStates::Idle);
				Multi_SetMaxWalkSpeed(DefaultWalkSpeed, DefaultCameraFOV);
			}
		}
	}
}

void USprintingComponent::Multi_SetMaxWalkSpeed_Implementation(float NewSpeed, float NewFOV)
{
	if (PlayerMovementComponentRef)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = NewSpeed;
		SetFOVTimer(NewFOV);
	}
}

void USprintingComponent::SetSprintingVariables()
{
	if (PlayerRef->CurrentMovementState != EMovementStates::Crouching)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = SprintingSpeed;
		PlayerRef->ChangeMovementState(EMovementStates::Sprinting);
		Multi_SetMaxWalkSpeed(SprintingSpeed, SprintingFOV);
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
	DefaultCameraFOV = PlayerRef->GetFollowCamera()->FieldOfView;
}


void USprintingComponent::SetFOVTimer(const float NewFOV)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(FOVTimer);
	FOVTimer.Invalidate();
		
	float TargetFOV = NewFOV;
	TimerManager.SetTimer(FOVTimer, [this, TargetFOV]()
	{
		SetFOV(TargetFOV);
			
	}, 0.01f, true);
}

void USprintingComponent::SetFOV(const float NewFOV)
{
	const float CurrentFOV = PlayerRef->GetFollowCamera()->FieldOfView;
	// Gets the difference of FOV between the given NewFOV and the current (ie 90FOV and 110FOV is 20DIF)
	if (FMath::Abs(CurrentFOV - NewFOV) >= 0.5f)
	{
		// Check if to move fov up or down
		float Multiplier = 1.0f;
		if (CurrentFOV > NewFOV)
		{
			Multiplier = -1.0f;
		}
		
		PlayerRef->GetFollowCamera()->FieldOfView = CurrentFOV +
			((GetWorld()->GetDeltaSeconds() * FOVChangeMultiplier) * Multiplier);
		
		//UE_LOG(LogTemp, Warning, TEXT("CURRENT FOV : %f"), PlayerRef->GetFollowCamera()->FieldOfView);

		if (FMath::Abs(CurrentFOV - NewFOV) <= 0.5f)
		{
			PlayerRef->GetFollowCamera()->FieldOfView = NewFOV;
			//UE_LOG(LogTemp, Warning, TEXT("END FOV : %f"), PlayerRef->GetFollowCamera()->FieldOfView);
		
			GetWorld()->GetTimerManager().ClearTimer(FOVTimer);
			FOVTimer.Invalidate();
		}
	}
}

