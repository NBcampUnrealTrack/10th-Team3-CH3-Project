// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, CurrentHealth, int32, MaxHealth);

UCLASS( ClassGroup=(Attribute), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category="Attribute")
	int32 HealthMax;
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	int32 HealthCurrent;

public:	
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	void Initailize(int32 NewHealth);

	void Heal(int32 HealAmount);

	void TakeDamage(int32 Damage);

	int32 GetHealth() const;
};
