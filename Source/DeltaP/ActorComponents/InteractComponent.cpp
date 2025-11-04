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

void UInteractComponent::TryUpdatingReferences()
{
	OwningActor = GetOwner();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void UInteractComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(OwningActor) && !IsValid(CameraManager))
	{
		TryUpdatingReferences();
	}
	
	if (IsActive() && OwningActor->HasLocalNetOwner()) return;
	
	FHitResult Hit = TryExecuteTrace();
	
	if (AActor* HitActor = Hit.GetActor())
	{
		if (HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			bool CanInteract = IInteract::Execute_CanInteract(HitActor);
			if (CanInteract)
			{
				OnSuccessfulHit(IInteract::Execute_GetInteractInfo(HitActor));
			}
		}
	}
}


void UInteractComponent::TryInteract()
{
	FHitResult Hit= TryExecuteTrace();

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor)) return;

	if (!HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass())) return;

	bool CanInteract = IInteract::Execute_CanInteract(HitActor);
	if (CanInteract)
	{
		FInteractionInfo InteractionInfo = IInteract::Execute_GetInteractInfo(HitActor);
		if (InteractionInfo.Hold)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,TEXT("HOLD TO BE WORKED ON"));
			}
		}
		else
		{
			OnInteractComplete();
			Server_InteractComplete(HitActor);
		}
	}
}

void UInteractComponent::BeginInteract()
{
}

void UInteractComponent::CancelInteract()
{
}

void UInteractComponent::Server_InteractComplete_Implementation(AActor* HitActor)
{
	IInteract::Execute_Interact(HitActor, OwningActor);
}

FHitResult UInteractComponent::TryExecuteTrace()
{
	if (!IsValid(OwningActor) && !IsValid(CameraManager))
	{
		TryUpdatingReferences();
	}
	
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

	return Hit;
}

void UInteractComponent::OnInteractComplete_Implementation()
{
}

void UInteractComponent::OnSuccessfulHit_Implementation(const FInteractionInfo& Info)
{
}
