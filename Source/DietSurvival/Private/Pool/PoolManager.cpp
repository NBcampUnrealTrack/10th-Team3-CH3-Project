// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolManager.h"

#include "Pool/PoolBase.h"
#include "Pool/PoolObjectComponent.h"

// Sets default values
APoolManager::APoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

APoolBase* APoolManager::AddPool(TSubclassOf<AActor> PoolObjectClass, int32 PoolSize)
{
	if (!PoolObjectClass) return nullptr;

	UClass* ObjectClass = PoolObjectClass.Get();
	if (TObjectPtr<APoolBase> const* ExistingPool = PoolsMap.Find(ObjectClass)) {
		return *ExistingPool;
	}

	if (PoolsMap.Contains(ObjectClass)) {
		return PoolsMap[ObjectClass];
	}

	AActor* TestActor = Cast<AActor>(ObjectClass->GetDefaultObject());
	if (!TestActor) {
		return nullptr;
	}

	UPoolObjectComponent* PoolComponent =
		TestActor->FindComponentByClass<UPoolObjectComponent>();

	if (!PoolComponent) {
		return nullptr;
	}

	APoolBase* NewPool = GetWorld()->SpawnActor<APoolBase>();
	if (!NewPool) return nullptr;

	NewPool->InitalizePool(PoolObjectClass, PoolSize);
	PoolsMap.Add(PoolObjectClass, NewPool);
	return NewPool;
}

AActor* APoolManager::GetPoolOjbect(UClass* PoolObjectClass)
{
	if (!PoolObjectClass) return nullptr;
	if (!PoolsMap.Contains(PoolObjectClass)) {
		return nullptr;
	}
	return PoolsMap[PoolObjectClass]->Acquire();
}

void APoolManager::ShrinkPool(UClass* PoolObjectClass, int32 NewPoolSize)
{
	if (!PoolObjectClass) return;
	if (!PoolsMap.Contains(PoolObjectClass)) {
		return;
	}
	PoolsMap[PoolObjectClass]->Shrink(NewPoolSize);
}


