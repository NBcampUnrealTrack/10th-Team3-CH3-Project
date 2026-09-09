#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DietGameMode.generated.h"

//적 AI 작업 후 주석제거
class AEnemySpawner;
class ADietGameState;

UCLASS()
class DIETSURVIVAL_API ADietGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//게임모드 인스턴스를 찾아주는 함수
	static ADietGameMode* Get(const UObject* WorldContext);

public:
	// lifecycle
	ADietGameMode();
	virtual void BeginPlay() override;

public:
	// functions
	// 레벨 시작 함수. 메인->전투 레벨 전환
	void StartLevel();

	// 레벨 종료 함수. 종료 조건: 플레이어 포만감 최대 or 타이머 종료
	void EndLevel(bool bWin);

	//스포너에게 스폰 간격/데이터 전달 후 웨이브 시작
	void CommandSpawn(float DummySpawnTime, FTableRowBase* DummyMonsterRow);

	// 웨이브 진입 판단 후 스폰 명령
	void NextWave(int32 Wave);

public:
	//델리게이트 바인딩 함수

	//다음 웨이브 진입 판단
	UFUNCTION()
	void HandleWaveIncrease(int32 Wave);

	//제한 시간 도달 시 호출->종료조건
	UFUNCTION()
	void HandleTimeUp();

	//플레이어 포만감 최대 시 호출
	UFUNCTION()
	void HandlePlayerDefeat(); //todo 함수 이름 변경

private:
	// variables
	UPROPERTY()
	TObjectPtr<AEnemySpawner> CachedEnemySpawner;

	UPROPERTY()
	TObjectPtr<ADietGameState> CachedDietGameState;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TObjectPtr<UDataTable> WaveSpawnDataTable;

	//플레이어, 컨트롤러 등록 임시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> DefaultPlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerController> DefaultPlayerControllerClass;
};
