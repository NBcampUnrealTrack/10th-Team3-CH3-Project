// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Component/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HealthMax = 0;
	HealthCurrent = 0;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::Initailize(int32 NewHealth)
{
	HealthMax = NewHealth;
	HealthCurrent = NewHealth;
}

void UHealthComponent::Heal(int32 HealAmount)
{
	if (HealAmount <= 0) return;
	HealthCurrent = FMath::Min(HealthMax, HealthCurrent + HealAmount);
}

void UHealthComponent::TakeDamage(int32 Damage)
{
	if (Damage <= 0) return;
	HealthCurrent -= Damage;

	if (HealthCurrent <= 0) {
		OnDeath.Broadcast();
	}
}

int32 UHealthComponent::GetHealth() const
{
	return HealthCurrent;
}

