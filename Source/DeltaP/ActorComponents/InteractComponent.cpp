// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"

#include "DeltaP/Interfaces/Interact.h"
#include "Kismet/GameplayStatics.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.TickInterval = 0.1f;
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
	
	if (!IsActive() && !OwningActor->HasLocalNetOwner()) return;

	if (IsInteracting) return;

	TryExecutingSuccessfulHit();
}

void UInteractComponent::TryExecutingSuccessfulHit()
{
	FHitResult Hit = ExecuteTrace();
	
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
	else
	{
		CancelInteract();
	}
}


void UInteractComponent::TryInteract()
{
	FHitResult Hit= ExecuteTrace();

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor)) return;

	if (!HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass())) return;

	bool CanInteract = IInteract::Execute_CanInteract(HitActor);
	if (CanInteract)
	{
		FInteractionInfo InteractionInfo = IInteract::Execute_GetInteractInfo(HitActor);
		if (InteractionInfo.Hold)
		{
			UWorld* World = GetWorld();
			if (!IsValid(World)) return;

			IsInteracting = true;
			
			TargetActor = HitActor;
			TargetActorHoldTime = InteractionInfo.HoldTime;
			World->GetTimerManager().SetTimer(HoldTimer, this, &ThisClass::OnHoldFinished, InteractionInfo.HoldTime, false);
			World->GetTimerManager().SetTimer(UIHoldTimer, this, &ThisClass::TryUpdateHoldValue, 0.01f, true);
		}
		else
		{
			OnInteractComplete();
			Server_InteractComplete(HitActor);
		}
	}
}

void UInteractComponent::OnHoldFinished()
{
	if (TargetActor)
	{
		OnInteractComplete();
		Server_InteractComplete(TargetActor);
		CancelInteract();
	}
}

void UInteractComponent::CancelInteract()
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	if (!HoldTimer.IsValid() && !World->GetTimerManager().IsTimerActive(HoldTimer)) return;
	OnCancelInteract();
	
	IsInteracting = false;
	World->GetTimerManager().ClearTimer(HoldTimer);
	World->GetTimerManager().ClearTimer(UIHoldTimer);
	TargetActor = nullptr;
	TargetActorHoldTime = 0.f;

	OnHoldValueUpdated(TargetActorHoldTime);
}

void UInteractComponent::TryUpdateHoldValue()
{
	OnHoldValueUpdated(TargetActorHoldTime);
}

void UInteractComponent::Server_InteractComplete_Implementation(AActor* HitActor)
{
	if (IsValid(HitActor) && HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	{
		IInteract::Execute_Interact(HitActor, OwningActor);
	}
}

FHitResult UInteractComponent::ExecuteTrace()
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

void UInteractComponent::OnCancelInteract_Implementation()
{
}

void UInteractComponent::OnHoldValueUpdated_Implementation(const float Value)
{
}
