// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolObjectBase.h"
#include "Pool/PoolBase.h"

// Sets default values
APoolObjectBase::APoolObjectBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Pool = nullptr;
	PoolState = EPoolObjectState::InPool;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void APoolObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoolObjectBase::OnAcquire(bool bNeedTick)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(bNeedTick);
}

void APoolObjectBase::OnRelease()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void APoolObjectBase::SetPool(APoolBase* NewPool)
{
	if (NewPool) {
		return;
	}
	Pool = NewPool;
}

void APoolObjectBase::ReturnToPool()
{
	if (!Pool) {
		return;
	}
	Pool->Release(this);
}

EPoolObjectState APoolObjectBase::GetPoolState() const
{
	return PoolState;
}

void APoolObjectBase::SetPoolState(EPoolObjectState NewState)
{
	PoolState = NewState;
}

