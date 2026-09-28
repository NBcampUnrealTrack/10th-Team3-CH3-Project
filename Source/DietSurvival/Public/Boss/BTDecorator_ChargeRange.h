// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ChargeRange.generated.h"

/**
 * 
 */
UCLASS()
class DIETSURVIVAL_API UBTDecorator_ChargeRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ChargeRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
};
