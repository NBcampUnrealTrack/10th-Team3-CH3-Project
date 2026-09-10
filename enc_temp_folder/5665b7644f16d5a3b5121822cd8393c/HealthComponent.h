// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Attribute), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

	int32 HealthMax;
	int32 HealthCurrent;

public:	
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	void Initailize(int32 NewHealth);

	void Heal(int32 HealAmount);

	void TakeDamage(int32 Damage);

	int32 GetHealth() const;
};
