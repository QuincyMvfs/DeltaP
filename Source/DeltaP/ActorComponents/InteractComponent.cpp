// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"

#include "DeltaP/Interfaces/Interact.h"
#include "Kismet/GameplayStatics.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.TickInterval = 0.1f;

	SetIsReplicated(true);
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	TryUpdatingReferences();

	// Only turn tick on for the client since this would mostly be for UI
	if (IsValid(OwningActor) && OwningActor->HasLocalNetOwner())
	{
		PrimaryComponentTick.bCanEverTick = true;
		PrimaryComponentTick.TickInterval = 0.1f;
	}
}

void UInteractComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwningActor) && !IsValid(CameraManager))
	{
		TryUpdatingReferences();
	}
	
	if (IsActive() && OwningActor->HasLocalNetOwner()) return;

	FVector StartLocation = CameraManager->GetCameraLocation();
	FVector EndLocation = (CameraManager->GetActorForwardVector() * InteractDistance) + StartLocation;

	FHitResult Hit;
	UKismetSystemLibrary::CapsuleTraceSingle(this,
		StartLocation,
		EndLocation,
		CapsuleRadius,
		0.0f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{OwningActor},
		DrawDebugType,
		Hit, true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugTraceDuration);
	
	if (AActor* HitActor = Hit.GetActor())
	{
		if (HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			bool CanInteract = IInteract::Execute_CanInteract(HitActor);
			if (CanInteract)
			{
				OnSuccessfulHit.Broadcast(IInteract::Execute_GetInteractInfo(HitActor));
			}
		}
	}
}

void UInteractComponent::TryUpdatingReferences()
{
	OwningActor = GetOwner();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}


