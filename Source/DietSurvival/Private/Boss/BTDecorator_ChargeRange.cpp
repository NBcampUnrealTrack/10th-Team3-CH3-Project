#include "Boss/BTDecorator_ChargeRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Enemy/DietBossEnemy.h"

UBTDecorator_ChargeRange::UBTDecorator_ChargeRange()
{
	NodeName = TEXT("Is Player In Charge Range");
	bNotifyTick = true;          // 매 틱 실행되게
	bAllowAbortLowerPri = true;  // 낮은 우선순위를 중단할 수 있게 허용
	bAllowAbortNone = true;

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

	bool bResult = Boss->IsPlayerInChargeRange();
	UE_LOG(LogTemp, Log, TEXT("[ChargeRange Decorator] Evaluated: %s"), bResult ? TEXT("true") : TEXT("false"));
	return bResult;
}
