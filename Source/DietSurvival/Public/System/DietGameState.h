#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DietGameState.generated.h"

class ADietGameMode;

//Wave 증가 시 호출되는 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveIncerease, int32, CurrentWave);
//최대 시간 도달 시 호출되는 델리게이트(종료조건)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeUp);

UCLASS()
class DIETSURVIVAL_API ADietGameState : public AGameState
{
	GENERATED_BODY()

public:
	// lifecycle
	ADietGameState();

protected:
	// lifecycle
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// getters
	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetCurrentWave() const { return CurrentWave; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	float GetElapsedTime() const { return ElapsedTime; }

	TWeakObjectPtr<APawn> GetPlayerRef();

public:
	// functions
	// 타이머 시작 함수
	void StartTimer();

	// 타이머 종료 함수
	void StopTimer();

	// 타이머 조건 확인 함수 -- 웨이브, 종료 조건
	UFUNCTION()
	void TickTimer();

	void SetPlayerRef(APawn* InPlayer);

public:
	// deligates
	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnWaveIncerease OnWaveIncrease;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimeUp OnTimeUp;

protected:
	// variables
	//현재 wave 정보
	UPROPERTY(BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave;

	// 게임 시작부터 흐른 시간
	//UPROPERTY(BlueprintReadOnly, Category = "Timer")
	//float ElapsedTime;

	// 타이머 콜백 간격
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float TimeInterval;

	// 다음 웨이브로 넘어가는 간격
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float WaveInterval;

	// 게임 진행 시간(승리 조건)
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MaxGameTime;

	//플레이어 캐릭터 참조 포인터
	UPROPERTY()	//GC가 추적 하도록
	TWeakObjectPtr<APawn> PlayerRef;	

private:
	// variables
	FTimerHandle ElapsedTimerHandle;
};
