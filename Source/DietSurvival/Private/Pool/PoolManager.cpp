// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolManager.h"

#include "Pool/PoolBase.h"
#include "Pool/PoolObjectBase.h"

// Sets default values
APoolManager::APoolManager()
{

}

// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

APoolBase* APoolManager::AddPool(TSubclassOf<APoolObjectBase> PoolingClass, int32 PoolSize)
{
	if (PoolsMap.Contains(PoolingClass)) {
		return PoolsMap[PoolingClass];
	}

	APoolBase* NewPool = GetWorld()->SpawnActor<APoolBase>();
	NewPool->InitalizePool(PoolingClass, PoolSize);
	PoolsMap.Add(PoolingClass, NewPool);
	return NewPool;
}

APoolObjectBase* APoolManager::GetPoolOjbect(TSubclassOf<APoolObjectBase> PoolingClass)
{
	if (!PoolsMap.Contains(PoolingClass)) {
		return nullptr;
	}
	return PoolsMap[PoolingClass]->Acquire();
}

void APoolManager::ShrinkPool(TSubclassOf<APoolObjectBase> PoolingClass, int32 NewPoolSize)
{
	if (!PoolsMap.Contains(PoolingClass)) {
		return;
	}
	PoolsMap[PoolingClass]->Shrink(NewPoolSize);
}


