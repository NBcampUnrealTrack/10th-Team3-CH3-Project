#include "Boss/BTDecorator_ChargeRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Enemy/DietBossEnemy.h"

UBTDecorator_ChargeRange::UBTDecorator_ChargeRange()
{
	NodeName = TEXT("Is Player In Charge Range");
}

bool UBTDecorator_ChargeRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return false;
	}

	ADietBossEnemy* Boss = Cast<ADietBossEnemy>(AIController->GetPawn());
	if (Boss == nullptr)
	{
		return false;
	}

	return Boss->IsPlayerInChargeRange();
}
