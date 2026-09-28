#include "Boss/BTTask_FireProjectiles.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Enemy/DietBossEnemy.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FireProjectiles::UBTTask_FireProjectiles()
{
	NodeName = TEXT("Fire Projectiles");
}

EBTNodeResult::Type UBTTask_FireProjectiles::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Log, TEXT("[BTTask_FireProjectiles] ExecuteTask called"));

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[BTTask_FireProjectiles] AIController is nullptr"));
		return EBTNodeResult::Failed;
	}

	CachedBoss = Cast<ADietBossEnemy>(AIController->GetPawn());
	if (CachedBoss == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[BTTask_FireProjectiles] CachedBoss is nullptr"));
		return EBTNodeResult::Failed;
	}

	//UE_LOG(LogTemp, Log, TEXT("[BTTask_FireProjectiles] Boss found, starting fire sequence"));

	CachedOwnerComp = &OwnerComp;
	RemainingShots = ShotCount;

	CachedBoss->PlayThrowSound();
	FireNext();

	return EBTNodeResult::InProgress;
}

void UBTTask_FireProjectiles::FireNext()
{
	//UE_LOG(LogTemp, Log, TEXT("[BTTask_FireProjectiles] FireNext called, Remaining: %d"), RemainingShots);

	if (CachedBoss == nullptr || RemainingShots <= 0)
	{
		if (CachedOwnerComp != nullptr)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(CachedBoss, 0);
	if (PlayerPawn != nullptr)
	{
		CachedBoss->FireProjectileAt(PlayerPawn->GetActorLocation());
	}

	RemainingShots--;

	if (RemainingShots > 0)
	{
		CachedBoss->GetWorldTimerManager().SetTimer(FireTimerHandle, this, &UBTTask_FireProjectiles::FireNext, ShotInterval, false);
	}
	else
	{
		if (CachedOwnerComp != nullptr)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
