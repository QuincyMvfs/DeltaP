// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::TakeDamage(const float Damage, const AActor* Instigator)
{
	Server_TakeDamage(Damage, Instigator);
}

void UHealthComponent::Server_TakeDamage_Implementation(const float Damage, const AActor* Instigator)
{
	if (Damage <= 0) return;

	float NewHealth = CurrentHealth - Damage;
	CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	
	Multi_TakeDamage(Damage, Instigator);
}

void UHealthComponent::Multi_TakeDamage_Implementation(const float Damage, const AActor* Instigator)
{
	OnDamagedEvent.Broadcast(Damage, Instigator);
}

void UHealthComponent::HealDamage(const float HealAmount, const AActor* Instigator)
{
	Server_HealDamage(HealAmount, Instigator);
}


void UHealthComponent::Server_HealDamage_Implementation(const float HealAmount, const AActor* Instigator)
{
	if (HealAmount <= 0) return;

	float NewHealth = CurrentHealth + HealAmount;
	CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	
	Multi_HealDamage(HealAmount, Instigator);
}

void UHealthComponent::Multi_HealDamage_Implementation(const float HealAmount, const AActor* Instigator)
{
	OnHealedEvent.Broadcast(HealAmount, Instigator);
}