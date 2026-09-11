// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/PoolBase.h"
#include "Pool/PoolObjectComponent.h"

// Sets default values
APoolBase::APoolBase()
{
	PrimaryActorTick.bCanEverTick = false;
	AllObjects = {};
	AvailableObjects = {};
	ObjectClassSaved = nullptr;
}

// Called when the game starts or when spawned
void APoolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoolBase::InitalizePool(TSubclassOf<AActor> ObjectClass, int32 PoolSize)
{
	if (ObjectClassSaved) {
		return;
	}

	if (!ObjectClass) return;

	ObjectClassSaved = ObjectClass;

	for (int32 i = 0; i < PoolSize; i++) {
		AActor* NewObject =
			GetWorld()->SpawnActor<AActor>(ObjectClassSaved);

		if (NewObject && NewObject->FindComponentByClass<UPoolObjectComponent>()) {
			UPoolObjectComponent* PoolObjectComponent = NewObject->FindComponentByClass<UPoolObjectComponent>();
			PoolObjectComponent->SetPool(this);
			AllObjects.Add(NewObject);
			AvailableObjects.Add(NewObject);
		}
	}
}

AActor* APoolBase::Acquire()
{
	if (!ObjectClassSaved) {
		return nullptr;
	}
	if (AvailableObjects.IsEmpty()) {
		AActor* NewObject =
			GetWorld()->SpawnActor<AActor>(ObjectClassSaved);
		UPoolObjectComponent* PoolObjectComponent = NewObject->FindComponentByClass<UPoolObjectComponent>();
		PoolObjectComponent->SetPool(this);
		AllObjects.Add(NewObject);
		PoolObjectComponent->SetIsInPool(false);
		return NewObject;
	}
	AActor* PopedObject = AvailableObjects.Pop();
	UPoolObjectComponent* PoolObjectComponent = PopedObject->FindComponentByClass<UPoolObjectComponent>();
	PoolObjectComponent->SetIsInPool(false);
	return PopedObject;
}

void APoolBase::Release(UPoolObjectComponent* Object)
{
	if (!ObjectClassSaved) return;

	if (!Object || Object->IsInPool()) {
		return;
	}
	AActor* PoolObjectOwner = Object->GetOwner();
	if (!PoolObjectOwner) {
		return;
	}
	Object->SetIsInPool(true);
	Object->OnRelease();
	AvailableObjects.Push(PoolObjectOwner);
}

void APoolBase::Shrink(int32 NewPoolSize)
{
	if (AllObjects.Num() < NewPoolSize) {
		return;
	}

	int32 DeleteCount = AllObjects.Num() - NewPoolSize;

	for (int32 I = 0; I < DeleteCount; I++) {
		if (AvailableObjects.IsEmpty()) {
			break;
		}
		AActor* PopedObject = AvailableObjects.Pop();
		AllObjects.Remove(PopedObject);
		PopedObject->Destroy();
	}
}

