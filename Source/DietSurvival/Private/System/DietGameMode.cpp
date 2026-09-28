#include "System/DietGameMode.h"
#include "System/DietGameState.h"
#include "System/DataTableSubsystem.h"
#include "System/EnemyDataRow.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerStatComponent.h"
#include "Enemy/EnemySpawner.h"
#include "Enemy/DietBossEnemy.h"
#include "UI/MainHUD.h"
#include "Components/CapsuleComponent.h"
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
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::BeginPlay] GameStateRef is null"));
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
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::BeginPlay] SpawnerRef is null"));
	}

	// GameMode가 아닌 다른 곳에서 this를 캡처하는건 위험할 수 있음.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{

		// SetTimerForNextTick() 함수 : 모든 객체의 BeginPlay()가 끝나고 실행됨
		// SetTimerForNextTick() 이걸 GameMode::BeginPlay 에 넣어라
		// (주의) 이걸 여러곳에서 호출한다면?
		// Race condition 발생 가능 -> 팀원과 사전에 조율해야됨
		//   -> 게임 인스턴스의 이벤트 버스로 해결 가능

		// 게임종료 델리게이트 구독
		APawn* Player = CachedDietGameState->GetPlayerRef().Get();
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::BeginPlay] PlyaerRef is null"));
		}
		else
		{
			UPlayerStatComponent* StatComp = Player->FindComponentByClass<UPlayerStatComponent>();
			if (StatComp != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("[DietGameMode::BeginPlay] StatComponent binding success"));
				StatComp->OnFullnessMax.AddDynamic(this, &ADietGameMode::HandleGameOver);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::BeginPlay] StatComponent is null"));
			}
		}
	});
	// 보스 스폰 포인트 설정
	TArray<AActor*> FoundSpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BossSpawn"), FoundSpawnPoints);
	if (FoundSpawnPoints.Num() > 0)
	{
		BossSpawnPoint = FoundSpawnPoints[0];
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] BossSpawnPoint not found"));
	}
	StartLevel();
}

void ADietGameMode::StartLevel()
{
	if (CachedDietGameState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::StartLevel] GameStateRef is null"));
		return;
	}
	CachedDietGameState->StartTimer();
}

void ADietGameMode::EndLevel(bool bWin)
{
	if (CachedEnemySpawner != nullptr)
	{
		CachedEnemySpawner->SpawnStop();
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode::EndLevel] SpawnStop called"));
	}

	CachedDietGameState->StopBossPhaseTimer();
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::EndLevel] 게임 종료 플레이어 %s"), bWin ? TEXT("승리") : TEXT("패배"));
}

void ADietGameMode::CommandSpawn(float InSpawnDuration, const FEnemyDataRow& MonsterRow)
{
	if (CachedEnemySpawner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::CommandSpawn] CachedEnemySpawner is nullptr "));
		return;
	}

	CachedEnemySpawner->SetSpawnTimeAndMonster(InSpawnDuration, MonsterRow);
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] call SetSpawnTimeAndMonster()"));
}

void ADietGameMode::CommandSpawn(const FEnemyDataRow& MonsterRow)
{
	if (CachedEnemySpawner == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode::CommandSpawn] CachedEnemySpawner is nullptr "));
		return;
	}

	CachedEnemySpawner->SetSpawnMonster(MonsterRow);
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::CommandSpawn] call SetSpawnMonster() "));
}

void ADietGameMode::NextWave(int32 Wave)
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::NextWave] Wave %d Start!!"), Wave);

	UDataTableSubsystem* DTS = UDataTableSubsystem::Get(this);
	if (DTS == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::NextWave] DataTableSussystem is nullptr"));
		return;
	}

	// Todo EnemyDataTable -- Row Name 고민 해보기 일단 임시로 E1, E2로 되어있음.
	// 테이블로우가 더 없을 때는 마지막 몬스터 데이터를 넘겨주도록 설계
	int32 RowCount = DTS->GetEnemyDataTable()->GetRowMap().Num();
	if (RowCount < Wave)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::NextWave] Wave count exceeded the data row count EnemyDataRow: E%d"), RowCount);
		Wave = RowCount;
	}
	FString RowNameString = FString::Printf(TEXT("E%d"), Wave);
	FName RowName = FName(*RowNameString);

	FEnemyDataRow* ED = DTS->GetEnemyRowByFName(RowName);
	if (ED == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DietGameMode::NextWave] EnemyDataRow is nullptr"));
		return;
	}
	SpawnDuration = ED->SpawnInterval;
	CommandSpawn(SpawnDuration, *ED);
}

void ADietGameMode::HandleWaveIncrease(int32 Wave)
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::HandleWaveIncrease] call NextWave(%d) "), Wave);
	NextWave(Wave);
}

void ADietGameMode::HandleTimeUp()
{
	CachedDietGameState->StopTimer();
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::HandleTimeUp] Time Up!, Spawn Boss"));

	FVector SpawnLocation = BossSpawnPoint != nullptr ? BossSpawnPoint->GetActorLocation() : FVector::ZeroVector;
	// 보스 소환
	ADietBossEnemy* SpawnedBoss = GetWorld()->SpawnActor<ADietBossEnemy>(BossClass, SpawnLocation, FRotator::ZeroRotator);

	if (SpawnedBoss == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode::HandleTimeUp] Failed to spawn boss"));
		return;
	}

	SpawnedBoss->SetActorHiddenInGame(false);
	SpawnedBoss->SetActorEnableCollision(true);
	SpawnedBoss->RunAI();
	//UE_LOG(LogTemp, Log, TEXT("After SetActorEnableCollision: %d"), (int32)SpawnedBoss->GetCapsuleComponent()->GetCollisionEnabled());
	// 보스 사망 델리게이트 바인딩
	SpawnedBoss->OnBossDeath.AddDynamic(this, &ADietGameMode::HandleBossDeath);

	// 보스 타이머 시작
	if (CachedDietGameState != nullptr)
	{
		CachedDietGameState->OnBossPhaseTimeUp.AddDynamic(this, &ADietGameMode::HandleBossPhaseTimeUp);
		CachedDietGameState->StartBossPhaseTimer();
	}

}

void ADietGameMode::HandleGameOver()
{
	// Todo 게임 종료 로직 통일하기
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Player Defeat"));
	EndLevel(false);
}

void ADietGameMode::HandleBossDeath()
{
	// Todo 게임 종료 로직 통일하기
	EndLevel(true);
	CachedDietGameState->SetLevelEnded(true);
	GetWorldTimerManager().SetTimer(
		ResultTimerHandle,
		FTimerDelegate::CreateUObject(this, &ADietGameMode::ShowResultUI, true),
		ResultDelay,
		false
	);
}

void ADietGameMode::HandleBossPhaseTimeUp()
{
	// Todo 게임 종료 로직 통일하기
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode::HandleBossPhaseTimeUp] Boss phase Timeup"));
	EndLevel(false);
}

void ADietGameMode::ShowResultUI(bool bWin)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC == nullptr) return;

	AMainHUD* MainHUD = Cast<AMainHUD>(PC->GetHUD());
	if (MainHUD == nullptr) return;

	MainHUD->ShowResult(bWin);
}
