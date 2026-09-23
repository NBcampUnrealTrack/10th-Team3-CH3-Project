#include "Boss/BTTask_Charge.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Enemy/DietBossEnemy.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Charge::UBTTask_Charge()
{
	NodeName = TEXT("Charge At Player");
	bNotifyTick = true;   // TickTask를 매 프레임 받으려면 필요
}

EBTNodeResult::Type UBTTask_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	CachedBoss = Cast<ADietBossEnemy>(AIController->GetPawn());
	if (CachedBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(CachedBoss, 0);
	if (PlayerPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	CachedBoss->ChargeAt(PlayerPawn->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void UBTTask_Charge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (CachedBoss == nullptr || !CachedBoss->IsCharging())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
