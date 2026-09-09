// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolManager.generated.h"

class APoolBase;

UCLASS()
class DIETSURVIVAL_API APoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolManager();

protected:
	UPROPERTY()
	TMap<TObjectPtr<UClass>, TObjectPtr<APoolBase>> PoolsMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	APoolBase* AddPool(TSubclassOf<AActor> PoolObjectClass, int32 PoolSize);

	AActor* GetPoolOjbect(UClass* PoolObjectClass);

	void ShrinkPool(UClass* PoolObjectClass, int32 NewPoolSize = 0);
};
