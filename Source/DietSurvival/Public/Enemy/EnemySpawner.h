// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class APoolManager;
class USceneComponent;
class UBoxComponent;
class ADietEnemyBase;
class ADietGameState;

struct FEnemyDataRow;

UCLASS()
class DIETSURVIVAL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	void SetSpawnTime(float NewSpawnTime);

	void SetSpawnMonster(FEnemyDataRow* NewMonsterRow);

	void SetSpawnTimeAndMonster(float NewSpawnTime, FEnemyDataRow* NewMonsterRow);

	void SpawnStop();

protected:
	TObjectPtr<ADietGameState> DietGameState;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY()
	TObjectPtr<APoolManager> PoolManager;

	UPROPERTY()
	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere)
	float DistanceSafeSpawn;

	float SpawnTime;

	FEnemyDataRow* MonsterRow;

	virtual void BeginPlay() override;

	FVector3d GetNewSpawnLocation() const;

	void StartSpawn();

	UFUNCTION()
	void SpawnEnemy();
};
