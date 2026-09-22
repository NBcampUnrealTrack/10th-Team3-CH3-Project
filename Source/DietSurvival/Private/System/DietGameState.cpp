// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DietGameState.h"

ADietGameState::ADietGameState()
{
	CurrentWave = 0;
	MyElapsedTime = 0.0f;
	TimeInterval = 1.0f;
	WaveInterval = 10.0f;
	MaxGameTime = 180.0f;
}

void ADietGameState::BeginPlay()
{
	Super::BeginPlay();
	//테스트용
	//StartTimer();
}

void ADietGameState::Tick(float DeltaTime)
{
}

//타이머 시작. 
void ADietGameState::StartTimer()
{
	GetWorldTimerManager().SetTimer(
		ElapsedTimerHandle,
		this,
		&ADietGameState::TickTimer,
		TimeInterval,
		true
	);
}

void ADietGameState::StopTimer()
{
	GetWorldTimerManager().ClearTimer(ElapsedTimerHandle);
}

void ADietGameState::StartBossPhaseTimer()
{
	BossPhaseeRemainigTime = BossPhaseTimeLimit;
	OnBossPhaseStarted.Broadcast();
	OnBossPhaseTimeChanged.Broadcast(BossPhaseeRemainigTime);
	GetWorldTimerManager().SetTimer(
		BossPhaseTimerHandle,
		this,
		&ADietGameState::TickBossPhaseTimer,
		1.0f,
		true
	);
}

void ADietGameState::StopBossPhaseTimer()
{
	GetWorldTimerManager().ClearTimer(BossPhaseTimerHandle);
}

//웨이브 증가와 제한 시간 조건 확인
void ADietGameState::TickTimer()
{
	MyElapsedTime += 1.0f;
	
	UpdateElapsedTime.Broadcast(MyElapsedTime);

	//UE_LOG(LogTemp, Log, TEXT("[DietGameState]시간 증가. ElapsedTime: %.0f"), MyElapsedTime);
	if (MyElapsedTime >= WaveInterval * CurrentWave || CurrentWave == 0)
	{
		CurrentWave++;
		OnWaveIncrease.Broadcast(CurrentWave);
		//테스트용
		UE_LOG(LogTemp, Log, TEXT("[DietGameState::TickTimer]Wave 증가. 현재 Wave: %d"), CurrentWave);
	}
	if (MyElapsedTime >= MaxGameTime)
	{
		OnTimeUp.Broadcast();
		GetWorldTimerManager().ClearTimer(ElapsedTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("[DietGameState::TickTimer]최대 시간 도달"));
	}
}

void ADietGameState::TickBossPhaseTimer()
{
	BossPhaseeRemainigTime = FMath::Max(BossPhaseeRemainigTime - 1.0f, 0.f);
	OnBossPhaseTimeChanged.Broadcast(BossPhaseeRemainigTime);
	UE_LOG(LogTemp, Log, TEXT("[DietGameState::TickBossPhase] 시간 감소. ElapsedTime: %.0f"), BossPhaseeRemainigTime);
	if (BossPhaseeRemainigTime <= 0.0f)
	{
		OnBossPhaseTimeUp.Broadcast();
		StopBossPhaseTimer();
	}
}

//플레이어 참조 설정
void ADietGameState::SetPlayerRef(APawn* InPlayer)
{
	if (InPlayer == nullptr)
	{
		return;
	}
	PlayerRef = InPlayer;
}

//플레이어 참조 반환
TWeakObjectPtr<APawn> ADietGameState::GetPlayerRef()
{
	return PlayerRef;
}


void ADietGameState::AddKill()
{
	++KillCount;
	OnKillCountChanged.Broadcast(KillCount);
}
