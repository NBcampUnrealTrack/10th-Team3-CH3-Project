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
	MonsterRow = {};
	PoolManager = nullptr;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	Initialize();

	if (bIsTest) {
		PoolManager->AddPool(TestClass, 10);
		MonsterRow.EnemyClass = TestClass;
		MonsterRow.Health = TestHealth;
		MonsterRow.PowerAttack = TestPowerAttack;
		if (FMath::IsNearlyZero(SpawnTime) || SpawnTime < 0) {
			SpawnTime = 3.f;
		}
		StartSpawn();
	}
}

void AEnemySpawner::Initialize()
{
	if (!PoolManager) {
		APoolManager* NewPoolManager = GetWorld()->SpawnActor<APoolManager>();
		if (NewPoolManager) {
			PoolManager = NewPoolManager;
		}
	}
	if (!DietGameState) {
		DietGameState = Cast<ADietGameState>(GetWorld()->GetGameState());
	}
}

void AEnemySpawner::SetSpawnTime(float NewSpawnTime)
{
	if (NewSpawnTime <= 0) return;

	SpawnTime = NewSpawnTime;

	if (MonsterRow.Health == 0) return;

	StartSpawn();

}
void AEnemySpawner::SetSpawnMonster(const FEnemyDataRow& NewMonsterRow)
{
	MonsterRow = NewMonsterRow;

	if (!PoolManager) {
		Initialize();
	}

	PoolManager->AddPool(MonsterRow.EnemyClass, 10);

	if (FMath::IsNearlyZero(SpawnTime)) return;

	StartSpawn();
}

void AEnemySpawner::SetSpawnTimeAndMonster(float NewSpawnTime, const FEnemyDataRow& NewMonsterRow)
{
	if (NewSpawnTime <= 0) return;

	SpawnTime = NewSpawnTime;
	MonsterRow = NewMonsterRow;

	if (!PoolManager) {
		Initialize();
	}

	PoolManager->AddPool(MonsterRow.EnemyClass, 10);

	StartSpawn();
}

void AEnemySpawner::ShrinkMonsters(const FEnemyDataRow& OldMonsterRow, int32 PoolSize)
{
	PoolManager->ShrinkPool(OldMonsterRow.EnemyClass, PoolSize);
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
	if (!CollisionBox) {
		UE_LOG(LogTemp, Warning,
			TEXT("AEnemySpawner::GetNewSpawnLocation, CollisionBox is Null"));
		return FVector(0);
	}

	if (!DietGameState) {
		UE_LOG(LogTemp, Warning,
			TEXT("AEnemySpawner::GetNewSpawnLocation, DietGameState is Null"));
		return FVector(0);
	}

	if (!DietGameState->GetPlayerRef().IsValid()) {
		UE_LOG(LogTemp, Warning,
			TEXT("AEnemySpawner::GetNewSpawnLocation, GetPlayerRef is Null"));
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
	AActor* PoolObject = PoolManager->GetPoolOjbect(MonsterRow.EnemyClass);
	if (!PoolObject) return;
	ADietEnemyBase* NewEnemy = Cast<ADietEnemyBase>(PoolObject);

	if (!NewEnemy) return;

	NewEnemy->InitAttritube(MonsterRow);
	FVector SpawnLocation = GetNewSpawnLocation();
	/*UE_LOG(
		LogTemp,
		Warning,
		TEXT("Location X : %f, Y: %f, Z: %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);*/
	NewEnemy->SetActorLocation(SpawnLocation);
	NewEnemy->RunAI();
}

