#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "AttackComponent.generated.h"

class UPlayerStatComponent;
class UNiagaraSystem;

//적에게 공격 적중했을 때 방송(UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackHit, AActor*, HitActor, float, DamageAmount);

// 재장전 시작 시 브로드캐스트 (UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadStart);

// 현재 탄약 변화 시 브로드캐스트 (UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentAmmoChanged, int32, NewCurrentAmmo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIETSURVIVAL_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttackComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 자동 공격 시작
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartAutoAttack();

	// 자동 공격 중지
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopAutoAttack();

	// 재장전 시작
	void ReloadAmmo();

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnAttackHit OnAttackHit;

	// 재장전 시작 시 호출됨 (UI에서 구독)
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnReloadStart OnReloadStart;

	// 현재 탄약 변화 시 호출됨 (UI에서 구독)
	UPROPERTY(BlueprintAssignable, Category = "Stat|Combat")
	FOnCurrentAmmoChanged OnCurrentAmmoChanged;

	// 플레이어캐릭터에서 호출
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetParallelSpreadMode(bool bEnable) { bUseParallelSpreadMode = bEnable; }

protected:
	// StatComponent를 못 찾았을 때 쓸 기본 공격 간격(혹시나)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float FallbackAttackInterval = 3.f;

	// 라인트레이스 사거리 (cm) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 1000.f;

	// 트레이스 판정에 쓸 콜리전 채널 (적 콜리전 프리셋에 맞게 조정 필요) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// 라인트레이스 시작 위치를 카메라에서 앞으로 얼마나 이동시킬지 (cm)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float MuzzleForwardOffset = 300.f;

	// 디버그용 트레이스 라인을 화면에 그릴지 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Debug", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebugTrace = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Debug", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo = 0;

	// 매번 GetOwner()->FindComponentByClass()를 호출하지 않도록 BeginPlay에서 캐싱
	UPROPERTY()
	TObjectPtr<UPlayerStatComponent> CachedStatComponent;

	// 집중공격 여부
	bool bUseParallelSpreadMode = false;

	// 집중공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float ParallelSpreadTotalWidth = 10.f;

	// 집중 공격 데미지 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float ParallelModeDamageMultiplier = 0.5f;

	// 옆으로 LateralOffset만큼 평행 이동한 위치에서 정면으로 직진 발사
	void FireParallelTrace(const FVector& Start, const FRotator& ViewRotation, float LateralOffset, float Range);

	// 탄 수에 따른 좌우 오프셋 목록 계산
	const TArray<float>& GetParallelOffsets(int32 ShotCount);

	TMap<int32, TArray<float>> CachedParallelOffsets;

	FTimerHandle AttackTimerHandle;

	FTimerHandle ReloadTimerHandle;

	//한 방향으로 라인트레이스 1회 발사 + 데미지 적용 
	void FireTraceInDirection(const FVector& Start, const FRotator& BaseViewRotation, float YawOffset, float Range);

	// 실제 라인트레이스 + 데미지 처리
	void PerformAttack();

	// 현재 스탯 기준으로 다음 공격을 스케줄
	void ScheduleNextAttack();

	// 현재 AttackSpeed를 반영한 실제 공격 간격 계산	
	float GetCurrentAttackInterval() const;

	void OnReloadFinished();

	//방향 단계에 따라 발사 각도 목록을 계산해서 반환.
	const TArray<float>& GetActiveDirectionAngles(int32 DirectionLevel);

	// 방향 단계(int) -> 그 단계의 각도 목록. 한 번 계산되면 게임이 끝날 때까지 재사용됨
	TMap<int32, TArray<float>> CachedDirectionAngles;

private:
	bool bIsReloading = false;

	// 총 발사 소리. AttackDirection 레벨에 따라 다른 소리를 구현할 수도 있을 것 같아서 배열로 선언.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Sounds", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USoundBase>> FireSounds;

	// 적을 맞췄을 때 히트마커와 동시에 재생될 소리.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Sounds", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HitSound = nullptr;

	// 총알 트레일 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> BulletTrailSystem;

	// 적을 맞추고 조금 뒤에 HitSound 재생
	FTimerHandle PlayHitTimer;
};
