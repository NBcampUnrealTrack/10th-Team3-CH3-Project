// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolObjectComponent.generated.h"

class APoolBase;

// 풀 오브젝트가 활성화되거나 비활성화될 때 브로드캐스트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoolActiveChanged, bool, bActive);

UCLASS( ClassGroup=(Pool), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UPoolObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPoolObjectComponent();

protected:
	bool bIsInPool = true;
	UPROPERTY()
	TObjectPtr<APoolBase> Pool;

public:	
	void OnAcquire(bool bIsNeedTick = false);
	void OnRelease();
	void SetPool(APoolBase* NewPool);
	void ReturnToPool();
	void SetIsInPool(bool IsInPool);
	
	bool IsInPool() const;

	UPROPERTY(BlueprintAssignable, Category = "Pool")
	FOnPoolActiveChanged OnPoolActiveChanged;
};
