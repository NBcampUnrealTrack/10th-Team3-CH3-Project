// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Pool/PoolManager.h"
#include "Pool/PoolObjectComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnTime = 0.f;
	DistanceSafeSpawn = 500.f;
	MonsterRow = nullptr;
	PoolManager = nullptr;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	APoolManager* NewPoolManager = GetWorld()->SpawnActor<APoolManager>();
	if (NewPoolManager) {
		PoolManager = NewPoolManager;
	}

}

void AEnemySpawner::SetSpawnTime(float NewSpawnTime)
{
	if (NewSpawnTime <= 0) return;

	SpawnTime = NewSpawnTime;

	if (!MonsterRow) {
		StartSpawn();
	}
}
void AEnemySpawner::SetSpawnMonster(FTableRowBase* NewMonsterRow)
{
	if (!NewMonsterRow) return;

	MonsterRow = NewMonsterRow;

	//Todo : MonsterRow에서 클래스 가져와서 Pool 할 것
	PoolManager->AddPool(nullptr, 10);

	if (FMath::IsNearlyZero(SpawnTime)) return;

	StartSpawn();
}

void AEnemySpawner::SetSpawnTimeAndMonster(float NewSpawnTime, FTableRowBase* NewMonsterRow)
{
	if (NewSpawnTime <= 0 || !NewMonsterRow) return;

	SpawnTime = NewSpawnTime;
	MonsterRow = NewMonsterRow;

	StartSpawn();
}

void AEnemySpawner::SpawnStop()
{
	if (GetWorldTimerManager().IsTimerActive(SpawnTimer)) {
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}


void AEnemySpawner::StartSpawn()
{
	if (!PoolManager) return;

	if (GetWorldTimerManager().IsTimerActive(SpawnTimer)) {
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}

	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		AEnemySpawner::SpawnEnemy,
		SpawnTime,
		true
	);
}

FVector3d AEnemySpawner::GetNewSpawnLocation() const
{
	//Todo : 플레이어 위치 알아내고, 일정 범위 밖에서 스폰 하도록 위치 설정
	return FVector3d();
}


void AEnemySpawner::SpawnEnemy()
{
	AActor* NewEnemy = PoolManager->GetPoolOjbect(nullptr);
	if (!NewEnemy) return;

	
}

