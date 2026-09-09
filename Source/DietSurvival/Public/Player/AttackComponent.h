#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "AttackComponent.generated.h"

class UPlayerStatComponent;

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

protected:
	// StatComponent를 못 찾았을 때 쓸 기본 공격 간격(초) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float FallbackAttackInterval = 1.f;

	// 라인트레이스 사거리 (cm) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 1000.f;

	// 트레이스 판정에 쓸 콜리전 채널 (적 콜리전 프리셋에 맞게 조정 필요) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;

	// 디버그용 트레이스 라인을 화면에 그릴지 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Debug", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebugTrace = true;

	// 매번 GetOwner()->FindComponentByClass()를 호출하지 않도록 BeginPlay에서 캐싱
	UPROPERTY()
	TObjectPtr<UPlayerStatComponent> CachedStatComponent;

	FTimerHandle AttackTimerHandle;

	//한 방향으로 라인트레이스 1회 발사 + 데미지 적용 
	void FireTraceInDirection(const FVector& Start, const FRotator& BaseViewRotation, float YawOffset, float Range);

	// 실제 라인트레이스 + 데미지 처리
	void PerformAttack();

	// 현재 스탯 기준으로 다음 공격을 스케줄
	void ScheduleNextAttack();

	// 현재 AttackSpeed를 반영한 실제 공격 간격 계산	
	float GetCurrentAttackInterval() const;

	/** 방향 단계(1~4)에 따라 활성화되는 각도 오프셋 목록을 반환.
	 * 1: 정면만 / 2: 정면+후면 / 3: 동서남북 4방향 / 4: 8방향 전체(45도 간격) */
	static TArray<float> GetActiveDirectionAngles(int32 DirectionLevel);
};
