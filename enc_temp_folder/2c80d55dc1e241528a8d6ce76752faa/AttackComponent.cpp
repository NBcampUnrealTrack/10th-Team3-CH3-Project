#include "Player/AttackComponent.h"
#include "Player/PlayerStatComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
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

		// 실제 카메라의 위치/방향을 가져옴
		if (const APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			if (const APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
			{
				PC->GetPlayerViewPoint(Start, ViewRotation);
			}
		}

		const float Range = CachedStatComponent ? CachedStatComponent->GetAttackRange() : AttackRange;
		const int32 DirectionCount = CachedStatComponent ? CachedStatComponent->GetAttackDirection() : 1;

		// 방향 개수만큼 정해진 각도로 동시에 발사
		for (const float YawOffset : GetActiveDirectionAngles(DirectionCount))
		{
			FireTraceInDirection(Start, ViewRotation, YawOffset, Range);
		}
	}

	// 다음 공격을 최신 AttackSpeed 기준으로 다시 스케줄
	ScheduleNextAttack();
}

void UAttackComponent::FireTraceInDirection(const FVector& Start, const FRotator& BaseViewRotation, float YawOffset, float Range)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// 기준 시야 방향(BaseViewRotation)에서 YawOffset만큼 좌우로 회전시킨 방향으로 발사
	FRotator DirectionRotation = BaseViewRotation;
	DirectionRotation.Yaw += YawOffset;

	const FVector Forward = DirectionRotation.Vector();
	const FVector End = Start + Forward * Range;

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
		// StatComponent가 없으면 임시로 기본 데미지(10)를 사용 (테스트용 보호 로직)
		const float DamageAmount = CachedStatComponent ? CachedStatComponent->GetAttackPower() : 10.f;

		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			DamageAmount,
			Owner->GetInstigatorController(),
			Owner,
			UDamageType::StaticClass()
		);

		UE_LOG(LogTemp, Log, TEXT("[AttackComponent] %s에게 %.1f 데미지 적용 (방향 오프셋 %.0f°)"),
			*HitResult.GetActor()->GetName(), DamageAmount, YawOffset);
	}
}

//방향 단계를 입력 받아 활성화되는 각도 오프셋 목록을 반환
TArray<float> UAttackComponent::GetActiveDirectionAngles(int32 DirectionLevel)
{
	// 1 미만으로 내려가지 않게 보정 (0단계, 음수 단계 같은 비정상 입력 방지)
	const int32 ClampedLevel = FMath::Max(DirectionLevel, 1);

	// 단계당 발사 개수 계산: 1단계=1발, 2단계=3발, 3단계=5발, 4단계=7발...
	const int32 ShotCount = 2 * ClampedLevel - 1;

	TArray<float> Angles;
	Angles.Reserve(ShotCount); // 몇 개 들어갈지 미리 알고 있으니 메모리 재할당 방지용으로 예약

	// 발사 개수가 1발이면 정면
	if (ShotCount <= 1)
	{
		Angles.Add(0.f);
		return Angles;
	}

	// 항상 고정된 ±30도(총 60도) 범위 안에서만 퍼짐. 단계가 올라가도 범위 자체는 안 넓어지고,
	// 그 안에 들어가는 발사체 개수만 촘촘해지는 방식
	constexpr float HalfSpreadAngle = 30.f;

	const float StepAngle = (HalfSpreadAngle * 2.f) / (ShotCount - 1);

	// i=0일 때 -30도(가장 왼쪽), i가 커질수록 오른쪽으로 이동, 마지막엔 +30도(가장 오른쪽)
	for (int32 i = 0; i < ShotCount; ++i)
	{
		Angles.Add(-HalfSpreadAngle + StepAngle * i);
	}

	return Angles;
}
