// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DietEnemyBase.h"
#include "DietBossEnemy.generated.h"


class ABossProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);

UCLASS()
class DIETSURVIVAL_API ADietBossEnemy : public ADietEnemyBase
{
	GENERATED_BODY()
public:
	ADietBossEnemy();

public:
	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossDeath OnBossDeath;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void FireProjectileAt(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ChargeAt(FVector TargetLocation);

	bool IsCharging() const { return bIsCharging; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ABossProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	int32 BossMaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float ChargeSpeed = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float KnockbackStrength = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MaxChargeDuration = 2.0f;

	bool bIsCharging = false;

	FTimerHandle ChargeTimeoutHandle;

protected:
	virtual void HandleDeath() override;

	virtual void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

	void EndCharge();
};
