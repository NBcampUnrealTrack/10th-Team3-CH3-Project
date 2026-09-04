// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolManager.generated.h"

class APoolBase;
class APoolObjectBase;

UCLASS()
class DIETSURVIVAL_API APoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolManager();

protected:
	UPROPERTY()
	TMap<TSubclassOf<APoolObjectBase>, TObjectPtr<APoolBase>> PoolsMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	APoolBase* AddPool(TSubclassOf<APoolObjectBase> PoolingClass, int32 PoolSize);

	APoolObjectBase* GetPoolOjbect(TSubclassOf<APoolObjectBase> PoolingClass);

	void ShrinkPool(TSubclassOf<APoolObjectBase> PoolingClass, int32 NewPoolSize = 0);
};
