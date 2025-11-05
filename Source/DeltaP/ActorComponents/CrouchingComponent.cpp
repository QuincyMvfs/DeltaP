// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchingComponent.h"

#include "Camera/CameraComponent.h"
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
			Multi_SetWalkSpeed(DefaultWalkSpeed, DefaultCameraZPos);
		}
	}
}

void UCrouchingComponent::Multi_SetWalkSpeed_Implementation(const float NewSpeed, const float NewCameraPosition)
{
	if (PlayerMovementComponentRef)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = NewSpeed;
		SetCrouchingCameraTimer(NewCameraPosition);
	}
}

void UCrouchingComponent::UpdateVariablesToCrouching()
{
	if (PlayerRef->CurrentMovementState != EMovementStates::Sprinting)
	{
		PlayerMovementComponentRef->MaxWalkSpeed = CrouchingSpeed;
		PlayerRef->ChangeMovementState(EMovementStates::Crouching);
		Multi_SetWalkSpeed(CrouchingSpeed, CrouchCameraZPos);
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
	DefaultCameraZPos = PlayerRef->GetFollowCamera()->GetRelativeLocation().Z;
}

void UCrouchingComponent::SetCrouchingCameraTimer(const float TargetPosition)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(CrouchingTimer);
	CrouchingTimer.Invalidate();
		
	float TargetPos = TargetPosition;
	TimerManager.SetTimer(CrouchingTimer, [this, TargetPos]()
	{
		SetCameraPosition(TargetPos);
			
	}, 0.01f, true);	
}

void UCrouchingComponent::SetCameraPosition(const float TargetPosition)
{
	const FVector CurrentCameraPosition = PlayerRef->GetFollowCamera()->GetRelativeLocation();
	float Difference = FMath::Abs(CurrentCameraPosition.Z - TargetPosition);
	// Gets the difference of FOV between the given NewFOV and the current (ie 90FOV and 110FOV is 20DIF)
	if (Difference >= 0.5f)
	{
		// Check if to move fov up or down
		float Multiplier = 1.0f;
		if (CurrentCameraPosition.Z > TargetPosition)
		{
			Multiplier = -1.0f;
		}
		
		float NewZPosition = CurrentCameraPosition.Z +
			((GetWorld()->GetDeltaSeconds() * CrouchingCameraSpeed) * Multiplier);

		FVector NewPosition = FVector(CurrentCameraPosition.X, CurrentCameraPosition.Y, NewZPosition);
		PlayerRef->GetFollowCamera()->SetRelativeLocation(NewPosition);
		
		UE_LOG(LogTemp, Warning, TEXT("CURRENT Z POS : %f"), PlayerRef->GetFollowCamera()->GetRelativeLocation().Z);

		if (Difference <= 0.5f)
		{
			PlayerRef->GetFollowCamera()->SetRelativeLocation(FVector(CurrentCameraPosition.X, CurrentCameraPosition.Y, TargetPosition));
			UE_LOG(LogTemp, Warning, TEXT("END Z POS : %f"), PlayerRef->GetFollowCamera()->GetRelativeLocation().Z);
		
			GetWorld()->GetTimerManager().ClearTimer(CrouchingTimer);
			CrouchingTimer.Invalidate();
		}
	}
}
