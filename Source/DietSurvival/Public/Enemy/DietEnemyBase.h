// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DietEnemyBase.generated.h"

class UBehaviorTree;
class ADietAIController;
class UHealthComponent;
class UPoolObjectComponent;
class UMinimapTrackComponent;

struct FEnemyDataRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDeath, FVector, SpawnLocation, int32, MyExp);

UCLASS()
class DIETSURVIVAL_API ADietEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADietEnemyBase();

	void InitAttritube(const FEnemyDataRow& EnemyDataRow);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		class AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void RunAI();

	virtual void StopAI();

	UPROPERTY(BlueprintAssignable, Category="Exp")
	FOnEnemyDeath OnEnemyDeath;

protected:

	UPROPERTY(VisibleAnywhere, Category="Attribute")
	int32 PowerAttack;

	int32 Exp;

	bool bIsDead = true;

	UPROPERTY()
	TObjectPtr<AController> ControllerLastAttacked;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandlePoolActive(bool bIsActive);

	UFUNCTION()
	virtual void HandleDeath();

	UFUNCTION()
	virtual void AttackToTarget(AActor* Target);

	UFUNCTION()
	void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(EditAnywhere, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<UMinimapTrackComponent> MinimapTrackComponent;

	UPROPERTY()
	TObjectPtr<ADietAIController> DietAIController;
	UPROPERTY()
	TObjectPtr<UPoolObjectComponent> PoolObjectComponent;
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	TObjectPtr<UHealthComponent> HealthComponent;

};
