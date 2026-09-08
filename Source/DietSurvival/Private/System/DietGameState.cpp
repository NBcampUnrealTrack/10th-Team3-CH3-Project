// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DietGameState.h"

ADietGameState::ADietGameState()
{
	CurrentWave = 1;
	ElapsedTime = 0.0f;
	TimeInteval = 1.0f;
	WaveInterval = 10.0f;
	MaxGameTime = 60.0f;
}

void ADietGameState::BeginPlay()
{
	//테스트용
	StartTimer();
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
		1.0f,
		true
	);
}

//웨이브 증가와 제한 시간 조건 확인
void ADietGameState::TickTimer()
{
	//Todo: 시간이 두배로 증가하는 원인 파악
	ElapsedTime += 0.5f;	// [임시조치]시간이 두배로 증가해서 0.5씩 증가하게 해둠
	//Todo: 로그 찍을 때 %f로 하면 오류가 발생하는지 알아보기
	UE_LOG(LogTemp, Log, TEXT("[DietGameState]시간 증가. ElapsedTime: %d"), ElapsedTime);
	if (ElapsedTime >= WaveInterval * CurrentWave)
	{
		CurrentWave++;
		OnWaveIncrease.Broadcast(CurrentWave);
		//테스트용
		UE_LOG(LogTemp, Log, TEXT("[DietGameState]Wave 증가. 현재 Wave: %d"), CurrentWave);
	}
	if (ElapsedTime >= MaxGameTime)
	{
		OnTimeUp.Broadcast();
		GetWorldTimerManager().ClearTimer(ElapsedTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("[DietGameState]최대 시간 도달"));
	}
}

void ADietGameState::SetPlayerRef(APawn* InPlayer)
{
	if (InPlayer == nullptr)
	{
		return;
	}
	PlayerRef = InPlayer;
}

TWeakObjectPtr<APawn> ADietGameState::GetPlayerRef()
{
	return PlayerRef;
}
