// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindPlayer.generated.h"

/**
 * 
 */

class ADietGameState;

UCLASS()
class DIETSURVIVAL_API UFindPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UFindPlayer();
	
protected:

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	TObjectPtr<ADietGameState> DietGameState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;
};
