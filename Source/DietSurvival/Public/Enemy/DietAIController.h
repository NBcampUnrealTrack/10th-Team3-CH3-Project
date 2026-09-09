// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DietAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class DIETSURVIVAL_API ADietAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADietAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	
};
