#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DietGameState.generated.h"

class ADietGameMode;

//Wave 증가 시 호출되는 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveIncerease, int32, CurrentWave);
//최대 시간 도달 시 호출되는 델리게이트(종료조건)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeUp);
//1초마다 브로드캐스트 하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateElapsedTime, float, InElapsedTime);
//보스 페이즈 타이머 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossPhaseTimeUp);

//보스 페이즈 시작·남은 시간·킬 수 (UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossPhaseStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseTimeChanged, float, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillCountChanged, int32, KillCount);

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

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetKillCount() const { return KillCount; }

	TWeakObjectPtr<APawn> GetPlayerRef();

public:
	// functions
	// 타이머 시작 함수
	void StartTimer();

	// 타이머 종료 함수
	void StopTimer();

	void StartBossPhaseTimer();
	void StopBossPhaseTimer();

	// 타이머 조건 확인 함수 -- 웨이브, 종료 조건
	UFUNCTION() 
	void TickTimer();

	UFUNCTION()
	void TickBossPhaseTimer();

	void SetPlayerRef(APawn* InPlayer);

	// 적이 죽을 때 호출. 킬 수를 올리고 알림
	void AddKill();

public:
	// delegates
	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnWaveIncerease OnWaveIncrease;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimeUp OnTimeUp;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FUpdateElapsedTime UpdateElapsedTime;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnBossPhaseTimeUp OnBossPhaseTimeUp;

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossPhaseStarted OnBossPhaseStarted;

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FOnBossPhaseTimeChanged OnBossPhaseTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Kill")
	FOnKillCountChanged OnKillCountChanged;

protected:
	// variables
	//현재 wave 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	int32 CurrentWave;

	// 게임 시작부터 흐른 시간
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	float MyElapsedTime;

	// 타이머 콜백 간격
	UPROPERTY(VisibleAnywhere, Category = "Test")
	float TimeInterval;

	// 다음 웨이브로 넘어가는 간격
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	float WaveInterval;

	// 게임 진행 시간(승리 조건)
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	float MaxGameTime;

	//플레이어 캐릭터 참조 포인터
	UPROPERTY()	//GC가 추적 하도록
	TWeakObjectPtr<APawn> PlayerRef;


	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	float BossPhaseeRemainigTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	float BossPhaseTimeLimit = 60.0f;

	UPROPERTY(VisibleAnywhere, Category = "Kill")
	int32 KillCount = 0;

private:
	// variables
	FTimerHandle ElapsedTimerHandle;
	FTimerHandle BossPhaseTimerHandle;
	bool bLevelEnded = false;

public:
	bool IsLevelEnded() const { return bLevelEnded; }
	void SetLevelEnded(bool bEnded) { bLevelEnded = bEnded; }
};
