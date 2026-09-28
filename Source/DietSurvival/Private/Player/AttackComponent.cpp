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

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		// 매 공격마다 반복 탐색하지 않도록 한 번만 찾아서 캐싱(포인터라서 자동으로 업데이트 됨)
		CachedStatComponent = Owner->FindComponentByClass<UPlayerStatComponent>();

		if (!CachedStatComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AttackComponent] Owner(%s)에서 PlayerStatComponent를 찾지 못함"), *Owner->GetName());
		}
	}

	CurrentAmmo = CachedStatComponent ? CachedStatComponent->GetMaxAmmo() : 1;

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
	//재장전 중이면 공격 X
	if (bIsReloading)
	{
		return;
	}

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
		//카메라 위치가 아닌 시야 방향으로 앞으로 이동한 곳을 발사 지점으로 설정.
		Start += ViewRotation.Vector() * MuzzleForwardOffset;

		// 사거리: StatComponent가 있으면 그 값을, 없으면 기본값을 사용
		const float Range = CachedStatComponent ? CachedStatComponent->GetAttackRange() : AttackRange;

		// 탄 수 : StatComponent가 있으면 그 값을, 없으면 1단계(정면만)를 사용
		const int32 DirectionCount = CachedStatComponent ? CachedStatComponent->GetAttackDirection() : 1;

		if (bUseParallelSpreadMode)
		{
			const float DamageMultiplier = (DirectionCount > 1) ? ParallelModeDamageMultiplier : 1.f;

			// 집중 공격: 각도는 그대로 정면 고정, 좌우로만 나란히 오프셋
			for (const float LateralOffset : GetParallelOffsets(DirectionCount))
			{
				FireParallelTrace(Start, ViewRotation, LateralOffset, Range, DamageMultiplier);
			}

			// 집중 공격 발사 소리 재생
			if (FireSounds.IsValidIndex(1))
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSounds[1], GetOwner()->GetActorLocation());
			}
		}
		else
		{
			// 기존 부채꼴 모드
			for (const float YawOffset : GetActiveDirectionAngles(DirectionCount))
			{
				FireTraceInDirection(Start, ViewRotation, YawOffset, Range);
			}

			// 총 발사 소리 재생
			if (FireSounds.IsValidIndex(0))
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSounds[0], GetOwner()->GetActorLocation());
			}
		}
	}

	// 현재 탄알 수 감소
	CurrentAmmo--;
	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);

	// 탄알이 다 떨어졌으면 재장전 후 바로 다음 공격 수행.
	if (CurrentAmmo <= 0)
	{
		ReloadAmmo();
	}
	else
	{
		// 다음 공격을 최신 AttackSpeed 기준으로 다시 스케줄
		ScheduleNextAttack();
	}

}

//지정된 한 방향으로 라인트레이스 1회
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

	//사거리에 따른 끝점 계산
	const FVector Forward = DirectionRotation.Vector();
	const FVector End = Start + Forward * Range;

	// 이펙트 그리는 용도 끝 좌표
	FVector TrailEnd = End;

	//플레이어는 충돌 판정에서 제외
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	//충돌이 일어나면 bHit = true
	FHitResult HitResult;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, QueryParams);

	// 총알 경로 이펙트
	if (bHit)
	{
		TrailEnd = HitResult.ImpactPoint;
	}
	
	if (BulletTrailSystem)
	{
		UNiagaraComponent* TrailComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			BulletTrailSystem,
			Start
		);

		if (TrailComp)
		{
			TrailComp->SetNiagaraVariableVec3(TEXT("User.BeamStart"), Start);
			TrailComp->SetNiagaraVariableVec3(TEXT("User.BeamEnd"), TrailEnd);
		}
	}

	//HitResult가 Enemy 태그를 가진 오브젝트와 충돌한 것일 때 true
	const bool bHitEnemy = bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(TEXT("Enemy"));

	if (bDrawDebugTrace)
	{
		//아무것도 안맞았을 때 초록
		FColor DebugColor = FColor::Green;
		if (bHit)
		{
			//적이 맞았으면 빨강, 바닥 등에 맞았으면 노랑
			DebugColor = bHitEnemy ? FColor::Red : FColor::Yellow;
		}

		DrawDebugLine(GetWorld(), Start, bHit ? HitResult.Location : End,
			DebugColor, false, 0.5f, 0, 2.f);
	}

	if (bHitEnemy)
	{
		// 보호용 코드
		const float DamageAmount = CachedStatComponent ? CachedStatComponent->GetAttackPower() : 10.f;

		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			DamageAmount,
			Owner->GetInstigatorController(),
			Owner,
			UDamageType::StaticClass()
		);

		OnAttackHit.Broadcast(HitResult.GetActor(), DamageAmount);

		// 히트 소리 재생
		if (HitSound)
		{
			TWeakObjectPtr<UAttackComponent> WeakThis(this);
			GetWorld()->GetTimerManager().SetTimer(PlayHitTimer, [WeakThis]() {
				if (WeakThis.IsValid())
				{
					UAttackComponent* StrongThis = WeakThis.Get();
					UGameplayStatics::PlaySound2D(StrongThis, StrongThis->HitSound);
				}
			},
			0.1f,
			false);
		}
	}
}

void UAttackComponent::FireParallelTrace(const FVector& Start, const FRotator& ViewRotation, float LateralOffset, float Range, float DamageMultiplier)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// 방향은 그대로 정면(ViewRotation). 시작점만 오른쪽 벡터로 LateralOffset만큼 이동
	const FVector RightVector = ViewRotation.RotateVector(FVector::RightVector);
	const FVector OffsetStart = Start + RightVector * LateralOffset;

	const FVector Forward = ViewRotation.Vector();
	const FVector End = OffsetStart + Forward * Range;

	// 이펙트 그리는 용도 끝 좌표
	FVector TrailEnd = End;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	FHitResult HitResult;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, OffsetStart, End, TraceChannel, QueryParams);
	const bool bHitEnemy = bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(TEXT("Enemy"));

	// 총알 경로 이펙트
	if (bHit)
	{
		TrailEnd = HitResult.ImpactPoint;
	}

	if (PowerAttackTrailSystem)
	{
		UNiagaraComponent* TrailComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			PowerAttackTrailSystem,
			Start
		);

		if (TrailComp)
		{
			TrailComp->SetNiagaraVariableVec3(TEXT("User.BeamStart"), Start);
			TrailComp->SetNiagaraVariableVec3(TEXT("User.BeamEnd"), TrailEnd);
		}
	}

	if (bDrawDebugTrace)
	{
		FColor DebugColor = FColor::Green;
		if (bHit)
		{
			DebugColor = bHitEnemy ? FColor::Red : FColor::Yellow;
		}
		DrawDebugLine(GetWorld(), OffsetStart, bHit ? HitResult.Location : End, DebugColor, false, 0.5f, 0, 2.f);
	}

	if (bHitEnemy)
	{
		const float BaseDamage = CachedStatComponent ? CachedStatComponent->GetAttackPower() : 10.f;

		// 데미지 감소 배율 적용
		const float DamageAmount = BaseDamage * DamageMultiplier;

		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(), DamageAmount,
			Owner->GetInstigatorController(), Owner, UDamageType::StaticClass()
		);

		OnAttackHit.Broadcast(HitResult.GetActor(), DamageAmount);

		// 히트 소리 재생
		if (HitSound)
		{
			TWeakObjectPtr<UAttackComponent> WeakThis(this);
			GetWorld()->GetTimerManager().SetTimer(PlayHitTimer, [WeakThis]() {
				if (WeakThis.IsValid())
				{
					UAttackComponent* StrongThis = WeakThis.Get();
					UGameplayStatics::PlaySound2D(StrongThis, StrongThis->HitSound);
				}
			},
				0.1f,
				false);
		}
	}
}

const TArray<float>& UAttackComponent::GetParallelOffsets(int32 ShotCount)
{
	const int32 ClampedCount = FMath::Max(ShotCount, 1);

	if (const TArray<float>* FoundOffsets = CachedParallelOffsets.Find(ClampedCount))
	{
		return *FoundOffsets;
	}

	TArray<float> NewOffsets;
	NewOffsets.Reserve(ClampedCount);

	if (ClampedCount <= 1)
	{
		// 1발일 땐 정중앙 하나만
		NewOffsets.Add(0.f);
	}
	else
	{
		// 항상 고정된 ParallelSpreadTotalWidth 안에서만 퍼짐
		const float StepOffset = ParallelSpreadTotalWidth / (ClampedCount - 1);
		const float StartOffset = -ParallelSpreadTotalWidth / 2.f;

		for (int32 i = 0; i < ClampedCount; ++i)
		{
			NewOffsets.Add(StartOffset + StepOffset * i);
		}
	}

	return CachedParallelOffsets.Add(ClampedCount, MoveTemp(NewOffsets));
}

const TArray<float>& UAttackComponent::GetActiveDirectionAngles(int32 DirectionLevel)
{
	// 1 미만으로 내려가지 않게 보정 (0단계, 음수 단계 같은 비정상 입력 방지)
	const int32 ClampedLevel = FMath::Max(DirectionLevel, 1);

	// 이미 이 단계를 계산해서 캐시에 넣어둔 적이 있는지 먼저 확인
	if (const TArray<float>* FoundAngles = CachedDirectionAngles.Find(ClampedLevel))
	{
		// 캐시에 있으면 재계산 없이 바로 그 데이터의 참조를 반환
		return *FoundAngles;
	}

	// 캐시에 없다면 이번 한 번만 계산함
	// 단계당 발사 개수 계산: 1단계=1발, 2단계=3발, 3단계=5발, 4단계=7발...
	const int32 ShotCount = ClampedLevel;

	TArray<float> NewAngles;
	NewAngles.Reserve(ShotCount); // 몇 개 들어갈지 미리 알고 있으니 메모리 재할당 방지용으로 예약

	if (ShotCount <= 1)
	{
		// 1발일 땐 분산시킬 필요 없이 정면 하나만
		NewAngles.Add(0.f);
	}
	else
	{
		// 항상 고정된 ±30도(총 60도) 범위 안에서만 퍼짐. 단계가 올라가도 범위는 안 넓어지고
		// 그 안에 들어가는 발사체 개수만 촘촘해지는 방식
		constexpr float HalfSpreadAngle = 30.f;
		const float StepAngle = (HalfSpreadAngle * 2.f) / (ShotCount - 1);

		for (int32 i = 0; i < ShotCount; ++i)
		{
			NewAngles.Add(-HalfSpreadAngle + StepAngle * i);
		}
	}

	// 계산 결과를 캐시에 저장. TMap::Add는 저장된 값 자체의 참조를 돌려주므로,
	// 그 참조를 그대로 반환하면 매번 새로 복사할 필요 없이 캐시된 데이터를 바로 가리키게 됨
	return CachedDirectionAngles.Add(ClampedLevel, MoveTemp(NewAngles));
}

void UAttackComponent::ReloadAmmo()
{
	// 이미 최대 탄약이거나 재장전 중이면 재장전할 필요 없음
	if (CurrentAmmo >= (CachedStatComponent? CachedStatComponent->GetMaxAmmo(): 1) || bIsReloading)
	{
		return;
	}
	bIsReloading = true;
	OnReloadStart.Broadcast();

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	Owner->GetWorldTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&UAttackComponent::OnReloadFinished,
		CachedStatComponent ? CachedStatComponent->GetReloadTime() : 1.f,
		false
	);

	if (ReloadSound)
	{
		UGameplayStatics::PlaySound2D(this, ReloadSound);
	}
}

void UAttackComponent::OnReloadFinished()
{
	if (CachedStatComponent)
	{
		CurrentAmmo = CachedStatComponent->GetMaxAmmo();
	}
	else
	{
		CurrentAmmo = 1; // 기본값
	}

	OnCurrentAmmoChanged.Broadcast(CurrentAmmo);

	bIsReloading = false;

	ScheduleNextAttack();	
}
