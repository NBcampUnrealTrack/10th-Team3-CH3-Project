// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DietEnemyBase.generated.h"

class UBehaviorTree;
class ADietAIController;
class UHealthComponent;
class UPoolObjectComponent;

UCLASS()
class DIETSURVIVAL_API ADietEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADietEnemyBase();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		class AActor* DamageCauser) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void RunAI();

	virtual void StopAI();
protected:

	int32 PowerAttack;

	virtual void BeginPlay() override;

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
	TObjectPtr<ADietAIController> DietAIController;
	UPROPERTY()
	TObjectPtr<UPoolObjectComponent> PoolObjectComponent;
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

};
