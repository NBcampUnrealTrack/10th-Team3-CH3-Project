// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolBase.generated.h"

class UPoolObjectComponent;

UCLASS()
class DIETSURVIVAL_API APoolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolBase();

protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AllObjects;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AvailableObjects;

	UPROPERTY()
	TObjectPtr<UClass> ObjectClassSaved;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void InitalizePool(
		TSubclassOf<AActor> ObjectClass,
		int32 PoolSize
	);

	AActor* Acquire();
	void Release(UPoolObjectComponent* Object);
	void Shrink(int32 NewPoolSize);

};
