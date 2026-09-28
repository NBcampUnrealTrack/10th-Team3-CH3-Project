// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/EnemyDataRow.h"
#include "EnemySpawner.generated.h"

class APoolManager;
class USceneComponent;
class UBoxComponent;
class ADietEnemyBase;
class ADietGameState;

UCLASS()
class DIETSURVIVAL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	void SetSpawnTime(float NewSpawnTime);

	void SetSpawnMonster(const FEnemyDataRow& NewMonsterRow);

	void SetSpawnTimeAndMonster(float NewSpawnTime, const FEnemyDataRow& NewMonsterRow);

	void ShrinkMonsters(const FEnemyDataRow& OldMonsterRow, int32 PoolSize = 0);

	void SpawnStop();

	void Initialize();

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UBoxComponent>> CollisionBoxArray;

	TObjectPtr<ADietGameState> DietGameState;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY()
	TObjectPtr<APoolManager> PoolManager;

	UPROPERTY()
	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere)
	float DistanceSafeSpawn;

	UPROPERTY(EditAnywhere, Category = "Test")
	float SpawnTime;

	FEnemyDataRow MonsterRow;

	virtual void BeginPlay() override;

	FVector3d GetNewSpawnLocation() const;

	void StartSpawn();

	UFUNCTION()
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, Category = "Test")
	bool bIsTest = true;
	UPROPERTY(EditAnywhere, Category="Test")
	TSubclassOf<ADietEnemyBase> TestClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "Test")
	int32 TestHealth = 100;
	UPROPERTY(EditAnywhere, Category = "Test")
	int32 TestPowerAttack = 10;
};
