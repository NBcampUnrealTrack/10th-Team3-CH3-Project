// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DietGameMode.h"
#include "System/DietGameState.h"
#include "System/DataTableSubsystem.h"
#include "System/EnemyDataRow.h"
#include "Player/PlayerCharacter.h"
#include "Enemy/EnemySpawner.h"
#include "Player/PlayerStatComponent.h"
#include "Kismet/GameplayStatics.h"

ADietGameMode* ADietGameMode::Get(const UObject* WorldContext)
{
	if (WorldContext == nullptr)
	{
		return nullptr;
	}

	UWorld* World = WorldContext->GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	return World->GetAuthGameMode<ADietGameMode>();
}

ADietGameMode::ADietGameMode()
{
	GameStateClass = ADietGameState::StaticClass();
	//DefaultPawnClass = APlayerCharacter::StaticClass();

	SpawnDuration = 1.0f;

	//----------임시-----------
	if (DefaultPlayerCharacterClass != nullptr)
	{
		DefaultPawnClass = DefaultPlayerCharacterClass;
	}

	if (DefaultPlayerControllerClass != nullptr)
	{
		PlayerControllerClass = DefaultPlayerControllerClass;
	}
	//----------임시-----------
}

void ADietGameMode::BeginPlay()
{
	Super::BeginPlay();

	//델리게이트 구독
	CachedDietGameState = GetGameState<ADietGameState>();
	if (CachedDietGameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] GameStateRef is null"));
		return;
	}
	CachedDietGameState->OnWaveIncrease.AddDynamic(this, &ADietGameMode::HandleWaveIncrease);
	CachedDietGameState->OnTimeUp.AddDynamic(this, &ADietGameMode::HandleTimeUp);

	//Spawner 캐시
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundSpawners);
	if (FoundSpawners.Num() > 0)
	{
		CachedEnemySpawner = Cast<AEnemySpawner>(FoundSpawners[0]);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] SpawnerRef is null"));
	}

	StartLevel();
}

void ADietGameMode::StartLevel()
{
	if (CachedDietGameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] GameStateRef is null"));
		return;
	}
	CachedDietGameState->StartTimer();
}

void ADietGameMode::EndLevel(bool bWin)
{
	//todo
		//플레이어 입력 정지
		//UI출력

	if (CachedEnemySpawner != nullptr)
	{
		CachedEnemySpawner->SpawnStop();
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] SpawnStop called"));
	}

	//종료 조건 추가 시 DietGameState 유효성 검사 추가 검토하기
	CachedDietGameState->StopTimer();
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] 게임 종료 플레이어 %s"), bWin ? TEXT("승리") : TEXT("패배"));
}

void ADietGameMode::CommandSpawn(float InSpawnDuration, const FEnemyDataRow& MonsterRow)
{
	if (CachedEnemySpawner == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] CachedEnemySpawner is nullptr "));
		return;
	}

	CachedEnemySpawner->SetSpawnTimeAndMonster(InSpawnDuration, MonsterRow);
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] call SetSpawnTimeAndMonster()"));
}

void ADietGameMode::CommandSpawn(const FEnemyDataRow& MonsterRow)
{
	if (CachedEnemySpawner == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] CachedEnemySpawner is nullptr "));
		return;
	}

	CachedEnemySpawner->SetSpawnMonster(MonsterRow);
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] call SetSpawnMonster() "));
}

void ADietGameMode::NextWave(int32 Wave)
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Next Wave: %d"), Wave);

	//Todo EnemyDataTable -- Row Name 고민 해보기 일단 임시로 E1, E2로 되어있음.
	FString RowNameString = FString::Printf(TEXT("E%d"), Wave);
	FName RowName = FName(*RowNameString);

	UDataTableSubsystem* DTS = UDataTableSubsystem::Get(this);
	if (DTS == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] DataTableSussystem is nullptr"));
		return;
	}
	FEnemyDataRow* ED = DTS->GetEnemyRowByFName(RowName);
	if (ED == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] EnemyDataRow is nullptr"));
		return;
	}
	CommandSpawn(SpawnDuration, *ED);
}

void ADietGameMode::HandleWaveIncrease(int32 Wave)
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Wave Increased: %d"), Wave);

	//첫 번째 웨이브 시작할 때 게임종료 델리게이트 구독
	if (Wave == 1)
	{
		APawn* Player = CachedDietGameState->GetPlayerRef().Get();
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("[DietGameMode] PlyaerRef is null"));
		}
		else
		{
			UPlayerStatComponent* StatComp = Player->FindComponentByClass<UPlayerStatComponent>();
			if (StatComp != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("[DietGameMode] StatComponent binding success"));
				StatComp->OnFullnessMax.AddDynamic(this, &ADietGameMode::HandleGameOver);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("[DietGameMode] StatComponent is null"));
			}
		}
	}

	NextWave(Wave);
}

void ADietGameMode::HandleTimeUp()
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Time Up!"));
	EndLevel(true);
}

void ADietGameMode::HandleGameOver()
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Player Defeat"));
	EndLevel(false);
}
