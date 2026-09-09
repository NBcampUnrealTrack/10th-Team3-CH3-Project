#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

// 포만감 변화 시 브로드캐스트 (UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFullnessChanged, float, Fullness, float, MaxFullness);

// 포만감이 최대치에 도달해 게임오버가 됐을 때 브로드캐스트 (게임모드에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullnessGameOver);

UENUM(BlueprintType)
enum class EPlayerStatType : uint8
{
	DefaultStat,    // 증강 적용이 구현되지 않은 경우 임시로 DefaultStat으로 설정
	MoveSpeed,
	AttackPower,
	AttackSpeed,
	Fullness,
	MaxFullness
};

// 플레이어의 모든 스탯(경험치, 레벨 제외)을 관리하는 컴포넌트.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIETSURVIVAL_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 포만감
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Fullness", meta = (AllowPrivateAccess = "true"))
	float Fullness = 0.f;

	// 포만감 최대치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat|Fullness", meta = (AllowPrivateAccess = "true"))
	float MaxFullness = 100.f;

	// 게임오버 여부
	bool bIsGameOver = false;

public:
	// 포만감 변화 시 호출됨 (UI 바인딩용)
	UPROPERTY(BlueprintAssignable, Category = "Stat|Fullness")
	FOnFullnessChanged OnFullnessChanged;

	// 포만감이 최대치에 도달해 게임오버가 되었을 때 호출됨
	UPROPERTY(BlueprintAssignable, Category = "Stat|Fullness")
	FOnFullnessGameOver OnGameOver;

	// 포만감을 Amount만큼 증가. 0~MaxFullness 범위로 클램프됨
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	void AddFullness(float Amount);

	// 데미지를 받은 만큼 포만감 증가
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	void ApplyDamage(float DamageAmount) { AddFullness(DamageAmount); }

	//GETTERS
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	float GetFullness() const { return Fullness; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	float GetMaxFullness() const { return MaxFullness; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	float GetFullnessRatio() const { return MaxFullness > 0.f ? Fullness / MaxFullness : 0.f; } // 포만감을 0~1 범위로 정규화한 값 (UI용)

	// 포만감이 최대치면 true 반환 (게임오버 상태)
	UFUNCTION(BlueprintCallable, Category = "Stat|Fullness")
	bool IsGameOver() const { return bIsGameOver; }


protected:
	// 기본 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat|Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 600.f;

	// 기본 공격력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat|Combat", meta = (AllowPrivateAccess = "true"))
	float AttackPower = 10.f;

	// 기본 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat|Combat", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed = 10.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Stat|Movement")
	float GetMoveSpeed() const { return MoveSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Movement")
	void SetMoveSpeed(float NewSpeed) { MoveSpeed = NewSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Stat|Combat")
	float GetAttackPower() const { return AttackPower; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Combat")
	void SetAttackPower(float NewAttackPower) { AttackPower = NewAttackPower; }

	UFUNCTION(BlueprintCallable, Category = "Stat|Combat")
	float GetAttackSpeed() const { return AttackSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Combat")
	void SetAttackSpeed(float NewAttackSpeed) { AttackSpeed = NewAttackSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Stat|Upgrade")
	void UpgradeStat(EPlayerStatType StatType, float Amount);
};
