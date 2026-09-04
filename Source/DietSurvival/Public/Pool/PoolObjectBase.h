// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolObjectBase.generated.h"

class APoolBase;

UENUM()
enum class EPoolObjectState : uint8
{
	InPool,
	Active
};

UCLASS()
class DIETSURVIVAL_API APoolObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolObjectBase();

protected:
	UPROPERTY()
	TObjectPtr<APoolBase> Pool;

	EPoolObjectState PoolState;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetPoolState(EPoolObjectState NewState);

	friend class APoolBase;

public:	

	virtual void OnAcquire(bool bNeedTick);
	virtual void OnRelease();

	void SetPool(APoolBase* NewPool);
	virtual void ReturnToPool();
	EPoolObjectState GetPoolState() const;
};
