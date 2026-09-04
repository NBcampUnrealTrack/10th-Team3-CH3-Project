// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolBase.generated.h"

class APoolObjectBase;

UCLASS()
class DIETSURVIVAL_API APoolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolBase();

protected:
	UPROPERTY()
	TArray<TObjectPtr<APoolObjectBase>> AllObjects;
	UPROPERTY()
	TArray<TObjectPtr<APoolObjectBase>> AvailableObjects;

	UPROPERTY()
	TSubclassOf<APoolObjectBase> ObjectClassSaved;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void InitalizePool(
		TSubclassOf<APoolObjectBase> ObjectClass,
		int32 PoolSize
	);

	APoolObjectBase* Acquire();
	void Release(APoolObjectBase* Object);
	void Shrink(int32 NewPoolSize);

};
