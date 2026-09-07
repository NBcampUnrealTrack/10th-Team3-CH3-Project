#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DietGameState.generated.h"

class ADietGameMode;

UCLASS()
class DIETSURVIVAL_API ADietGameState : public AGameState
{
	GENERATED_BODY()

public:
	ADietGameState();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetCurrentWave() const { return CurrentWave; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	float GetElapsedTime() const { return ElapsedTime; }

protected:
	//현재 wave 정보
	UPROPERTY(BlueprintReadOnly, Category="Wave")
	int32 CurrentWave;

	// 게임 시작부터 흐른 시간
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	float ElaspedTime;

	// 다음 웨이브로 넘어가는 간격
	UPROPERTY(EditDefaultsOnly, Category="Wave")
	float WaveInterval;

	// 게임 진행 시간. 
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MaxGameTime;

private:
	ADietGameMode* GetDietGameMode() const;

};
