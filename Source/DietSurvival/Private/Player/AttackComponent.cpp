#include "Player/AttackComponent.h"
#include "Player/PlayerStatComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		// 매 공격마다 반복 탐색하지 않도록 한 번만 찾아서 캐싱
		CachedStatComponent = Owner->FindComponentByClass<UPlayerStatComponent>();

		if (!CachedStatComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AttackComponent] Owner(%s)에서 PlayerStatComponent를 찾지 못함"), *Owner->GetName());
		}
	}

	StartAutoAttack();
}

void UAttackComponent::StartAutoAttack()
{
	ScheduleNextAttack();
}

void UAttackComponent::StopAutoAttack()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

float UAttackComponent::GetCurrentAttackInterval() const
{
	if (!CachedStatComponent)
	{
		return FallbackAttackInterval;
	}

	// 0 이하로 내려가면 타이머가 무의미해지므로 최소값으로 클램프
	return FMath::Max(CachedStatComponent->GetAttackSpeed(), 0.05f);
}

void UAttackComponent::ScheduleNextAttack()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	Owner->GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&UAttackComponent::PerformAttack,
		GetCurrentAttackInterval(),
		false
	);
}

void UAttackComponent::PerformAttack()
{
	AActor* Owner = GetOwner();

	if (Owner)
	{
		FVector Start = Owner->GetActorLocation();
		FRotator ViewRotation = Owner->GetActorRotation();

	
		if (const APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			if (const APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
			{
				PC->GetPlayerViewPoint(Start, ViewRotation);
			}
		}

		const FVector Forward = ViewRotation.Vector();
		const FVector End = Start + Forward * AttackRange;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);

		FHitResult HitResult;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);

		if (bDrawDebugTrace)
		{
			DrawDebugLine(GetWorld(), Start, bHit ? HitResult.Location : End,
				bHit ? FColor::Red : FColor::Green, false, 0.5f, 0, 2.f);
		}

		if (bHit && HitResult.GetActor())
		{
			const float DamageAmount = CachedStatComponent->GetAttackPower();

			UGameplayStatics::ApplyDamage(
				HitResult.GetActor(),
				DamageAmount,
				Owner->GetInstigatorController(),
				Owner,
				UDamageType::StaticClass()
			);

			UE_LOG(LogTemp, Log, TEXT("[AttackComponent] %s에게 %.1f 데미지 적용"),
				*HitResult.GetActor()->GetName(), DamageAmount);
		}
	}

	// 다음 공격을 최신 AttackSpeed 기준으로 다시 스케줄
	ScheduleNextAttack();
}
