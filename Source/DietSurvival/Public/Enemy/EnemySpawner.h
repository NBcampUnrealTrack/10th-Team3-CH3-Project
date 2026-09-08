// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class APoolManager;
class USceneComponent;
class UBoxComponent;
class ADietEnemyBase;

UCLASS()
class DIETSURVIVAL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	void SetSpawnTime(float NewSpawnTime);

	void SetSpawnMonster(FTableRowBase* NewMonsterRow);

	void SetSpawnTimeAndMonster(float NewSpawnTime, FTableRowBase* NewMonsterRow);

	void SpawnStop();

protected:

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

	FTableRowBase* MonsterRow;

	virtual void BeginPlay() override;

	FVector3d GetNewSpawnLocation() const;

	void StartSpawn();

	UFUNCTION()
	void SpawnEnemy();
};
