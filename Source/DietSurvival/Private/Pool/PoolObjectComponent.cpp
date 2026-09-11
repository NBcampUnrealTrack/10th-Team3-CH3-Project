// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolObjectComponent.h"
#include "Pool/PoolBase.h"

// Sets default values for this component's properties
UPoolObjectComponent::UPoolObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPoolObjectComponent::OnAcquire(bool bIsNeedTick)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->SetActorHiddenInGame(false);
	Owner->SetActorEnableCollision(true);
	Owner->SetActorTickEnabled(bIsNeedTick);
}

void UPoolObjectComponent::OnRelease()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->SetActorHiddenInGame(true);
	Owner->SetActorEnableCollision(false);
	Owner->SetActorTickEnabled(false);
}

bool UPoolObjectComponent::IsInPool() const
{
	return bIsInPool;
}

void UPoolObjectComponent::SetPool(APoolBase* NewPool)
{
	if (Pool || !NewPool) return;
	Pool = NewPool;
}

void UPoolObjectComponent::ReturnToPool()
{
	if (!Pool) {
		UE_LOG(LogTemp, Warning,
			TEXT("UPoolObjectComponent::ReturnToPool, Pool is Null"));
		return;
	}
	Pool->Release(this);
}

void UPoolObjectComponent::SetIsInPool(bool IsInPool)
{
	bIsInPool = IsInPool;
}

