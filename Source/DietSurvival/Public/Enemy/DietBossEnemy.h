// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DietEnemyBase.h"
#include "DietBossEnemy.generated.h"


class ABossProjectile;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, int32, CurrentHealth, int32, MaxHealth);

UCLASS()
class DIETSURVIVAL_API ADietBossEnemy : public ADietEnemyBase
{
	GENERATED_BODY()
public:
	ADietBossEnemy();

public:
	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossDeath OnBossDeath;

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossHealthChanged OnBossHealthChanged;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void FireProjectileAt(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ChargeAt(FVector TargetLocation);

	bool IsCharging() const { return bIsCharging; }

	UFUNCTION(BlueprintPure, Category = "Attack")
	bool IsPlayerInChargeRange() const;

	void FaceTowardsPlayer();

	void PlayThrowSound();

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

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float ChargeRange = 300.0f;

	FTimerHandle ChargeTimeoutHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 BossPowerAttack = 20;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> SpawnSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ThrowSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ChargeSound;

protected:
	virtual void HandleDeath() override;

	UFUNCTION()
	void HandleHealthChanged(int32 CurrentHealth, int32 MaxHealth);

	virtual void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

public:
	void EndCharge();
};
