#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Charge.generated.h"

class ADietBossEnemy;


UCLASS()
class DIETSURVIVAL_API UBTTask_Charge : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Charge();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<ADietBossEnemy> CachedBoss;
};
