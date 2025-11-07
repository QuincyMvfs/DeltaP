// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamaged, float, Damage, const AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed, float, Damage, const AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, const AActor*, Instigator);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELTAP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamagedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;
	
	UFUNCTION(BlueprintCallable, Category = "Health|Damage")
	void TakeDamage(const float Damage, const AActor* Instigator);

	UFUNCTION(Server, Unreliable, Category = "Health|Damage|Server")
	void Server_TakeDamage(const float Damage, const AActor* Instigator);

	UFUNCTION(NetMulticast, Unreliable, Category = "Health|Damage|Multi")
	void Multi_TakeDamage(const float Damage, const AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Health|Heal")
	void HealDamage(const float HealAmount, const AActor* Instigator);

	UFUNCTION(Server, Unreliable, Category = "Health|Heal|Server")
	void Server_HealDamage(const float HealAmount, const AActor* Instigator);

	UFUNCTION(NetMulticast, Unreliable, Category = "HHealth|Heal|Multi")
	void Multi_HealDamage(const float HealAmount, const AActor* Instigator);
	
		
};
