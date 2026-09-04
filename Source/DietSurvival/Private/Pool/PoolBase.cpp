// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolBase.h"
#include "Pool/PoolObjectBase.h"

// Sets default values
APoolBase::APoolBase()
{
	AllObjects = {};
	AvailableObjects = {};
	ObjectClassSaved = nullptr;
}

// Called when the game starts or when spawned
void APoolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoolBase::InitalizePool(TSubclassOf<APoolObjectBase> ObjectClass, int32 PoolSize)
{
	if (ObjectClassSaved) {
		return;
	}
	ObjectClassSaved = ObjectClass;
	for (int32 i = 0; i < PoolSize; i++) {
		APoolObjectBase* NewObject =
			GetWorld()->SpawnActor<APoolObjectBase>(ObjectClass);

		if (NewObject) {
			NewObject->SetPool(this);
			AllObjects.Add(NewObject);
			AvailableObjects.Add(NewObject);
		}
	}
}

APoolObjectBase* APoolBase::Acquire()
{
	if (AvailableObjects.IsEmpty()) {
		APoolObjectBase* NewObject =
			GetWorld()->SpawnActor<APoolObjectBase>(ObjectClassSaved);
		NewObject->SetPool(this);
		AllObjects.Add(NewObject);
		NewObject->SetPoolState(EPoolObjectState::Active);
		return NewObject;
	}
	APoolObjectBase* PopedObject = AvailableObjects.Pop();
	PopedObject->SetPoolState(EPoolObjectState::Active);
	return PopedObject;
}

void APoolBase::Release(APoolObjectBase* Object)
{
	if (!Object) {
		return;
	}
	if (Object->GetPoolState() == EPoolObjectState::InPool) {
		return;
	}
	Object->SetPoolState(EPoolObjectState::InPool);
	Object->OnRelease();
	AvailableObjects.Push(Object);
}

