// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireProjectiles.generated.h"

class ADietBossEnemy;
class UBehaviorTreeComponent;

UCLASS()
class DIETSURVIVAL_API UBTTask_FireProjectiles : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FireProjectiles();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void FireNext();

	UPROPERTY(EditAnywhere, Category = "Fire")
	int32 ShotCount = 3;

	UPROPERTY(EditAnywhere, Category = "Fire")
	float ShotInterval = 0.3f;

	int32 RemainingShots = 0;

	UPROPERTY()
	TObjectPtr<ADietBossEnemy> CachedBoss;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	FTimerHandle FireTimerHandle;
	
};
