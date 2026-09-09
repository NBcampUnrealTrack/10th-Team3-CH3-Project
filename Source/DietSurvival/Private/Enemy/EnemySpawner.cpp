// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "Pool/PoolManager.h"
#include "Enemy/DietEnemyBase.h"
#include "System/DietGameState.h"


AEnemySpawner::AEnemySpawner()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	if (Scene) {
		SetRootComponent(Scene);
	}
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	if (CollisionBox) {
		CollisionBox->SetupAttachment(Scene);
	}


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
	DietGameState = Cast<ADietGameState>(GetWorld()->GetGameState());
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

	//Todo : MonsterRow에서 클래스 가져와서 Pool 할 것
	PoolManager->AddPool(nullptr, 10);

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
		&AEnemySpawner::SpawnEnemy,
		SpawnTime,
		true
	);
}

FVector3d AEnemySpawner::GetNewSpawnLocation() const
{
	if (!CollisionBox) return FVector(0);
	if (!DietGameState) return FVector(0);

	if (!DietGameState->GetPlayerRef().IsValid()) {
		return FVector(0);
	}

	FVector PlayerLocation = DietGameState->GetPlayerRef().Get()->GetActorLocation();

	FVector SpawnerLocation = GetActorLocation();
	FVector BoxExtent = CollisionBox->GetScaledBoxExtent();

	float DistSuqredSafeDistance = DistanceSafeSpawn * DistanceSafeSpawn;

	while (true)
	{
		FVector SpawnLocation = FVector(
			FMath::RandRange(SpawnerLocation.X - BoxExtent.X, SpawnerLocation.X + BoxExtent.X),
			FMath::RandRange(SpawnerLocation.Y - BoxExtent.Y, SpawnerLocation.Y + BoxExtent.Y),
			SpawnerLocation.Z
		);

		float DistSuqredDistanceFromPlayer = FVector::DistSquared(PlayerLocation, SpawnLocation);

		if (DistSuqredDistanceFromPlayer <= DistSuqredSafeDistance) {
			continue;
		}

		return SpawnLocation;
	}
}


void AEnemySpawner::SpawnEnemy()
{
	//Todo : MonsterRow 에서 몬스터 클래스 가져오기
	AActor* PoolObject = PoolManager->GetPoolOjbect(nullptr);
	if (!PoolObject) return;
	ADietEnemyBase* NewEnemy = Cast<ADietEnemyBase>(PoolObject);

	if (!NewEnemy) return;

	NewEnemy->SetActorLocation(GetNewSpawnLocation());
	NewEnemy->RunAI();
}

