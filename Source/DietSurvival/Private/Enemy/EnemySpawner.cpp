// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "Pool/PoolManager.h"
#include "Enemy/DietEnemyBase.h"
#include "System/DietGameState.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawner::AEnemySpawner()
{
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	if (Scene) {
		SetRootComponent(Scene);
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
	TArray<AActor*> FoundPoolManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APoolManager::StaticClass(), FoundPoolManagers);

	if (FoundPoolManagers.Num() > 0)
	{
		PoolManager = Cast<APoolManager>(FoundPoolManagers[0]);
	}
	else
	{
		PoolManager = GetWorld()->SpawnActor<APoolManager>();
	}
	if (!DietGameState) {
		DietGameState = Cast<ADietGameState>(GetWorld()->GetGameState());
	}

	GetComponents<UBoxComponent>(CollisionBoxArray);
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
	if (CollisionBoxArray.IsEmpty()) {
		UE_LOG(LogTemp, Warning,
			TEXT("AEnemySpawner::GetNewSpawnLocation, CollisionBoxArray is Empty"));
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

	int32 RandomIndex = FMath::RandRange(0, (CollisionBoxArray.Num() - 1));
	UBoxComponent* SelectedBox = CollisionBoxArray[RandomIndex];
	FVector BoxLocation = SelectedBox->GetComponentLocation();
	FVector BoxExtent = SelectedBox->GetScaledBoxExtent();

	float DistSuqredSafeDistance = DistanceSafeSpawn * DistanceSafeSpawn;

	while (true)
	{
		FVector SpawnLocation = FVector(
			FMath::RandRange(BoxLocation.X - BoxExtent.X, BoxLocation.X + BoxExtent.X),
			FMath::RandRange(BoxLocation.Y - BoxExtent.Y, BoxLocation.Y + BoxExtent.Y),
			BoxLocation.Z
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

