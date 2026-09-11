#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DietPlayerState.generated.h"

class UAugmentManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpChangedSignature, float, CurrentExp, float, MaxExp);

UCLASS()
class DIETSURVIVAL_API ADietPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADietPlayerState();

	// 캐릭터가 Amount만큼의 경험치를 획득
	void GainExp(int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Exp")
	FOnLevelUpSignature OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Exp")
	FOnExpChangedSignature OnExpChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diet|Augment")
	TObjectPtr<UAugmentManagerComponent> AugmentManager;
	
	FORCEINLINE int32 GetCurrentLevel() { return Level; }

	FORCEINLINE int32 GetCurrentExp() { return Exp; }

	FORCEINLINE int32 GetMaxExp() { return MaxExp; }

protected:
	void ApplyExp(float Amount);

	void LevelUp();

	float CalculateTickExp(float DeltaTime);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	// ----- 경험치, 레벨 -----

	float Exp = 0;
	float MaxExp = 10;
	int32 Level = 1;

	// ----- 경험치 대기열(점진적 경험치 증가) -----

	// 초당 적용할 경험치
	UPROPERTY(EditDefaultsOnly, Category = "EXP")
	float ExpAbsorbRate = 5.f;

	float PendingExp = 0.f;

	// ----- 경험치 증가 테스트용 -----
	FTimerHandle TestExpTimer;
	void TestGainExp();
};
