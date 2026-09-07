#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DietGameMode.generated.h"

//class AMonsterSpawner;

UCLASS()
class DIETSURVIVAL_API ADietGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADietGameMode();

	virtual void BeginPlay() override;

	//스포너에게 스폰 간격/데이터 전달 후 웨이브 시작
	void CommandSpawn();
	//다음 웨이브 진입
	void NextWave(int32 Wave);
	//제한 시간 도달 시 호출
	void OnTimeUp();
	//레벨업 시 pause 호출
	void OnPlayerLevelUp();

protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	//TSubclassOf<AMonsterSpawner> MonsterSpawnerClass;

	//UPROPERTY()
	//TObjectPtr<AMonsterSpawner> MonsterSpawner;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	//TObjectPtr<UDataTable> WaveSpawnDataTable;

private:
	//ADietGameState* GetDietGameState() const;
};
