// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DietGameMode.h"
#include "System/DietGameState.h"
//#include "PlayerCharacter.h"

ADietGameMode::ADietGameMode()
{
	GameStateClass = ADietGameState::StaticClass();
	//DefaultPawnClass = ADietCharacter::StaticClass();
}

void ADietGameMode::BeginPlay()
{
	Super::BeginPlay();

	//델리게이트 구독
	DietGameState = GetGameState<ADietGameState>();
	if (DietGameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] GameStateRef is null"));
		return;
	}
	DietGameState->OnWaveIncrease.AddDynamic(this, &ADietGameMode::HandleWaveIncrease);
	DietGameState->OnTimeUp.AddDynamic(this, &ADietGameMode::HandleTimeUp);

	StartLevel();
}

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

void ADietGameMode::StartLevel()
{
	if (DietGameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] GameStateRef is null"));
	}
	DietGameState->StartTimer();
}

void ADietGameMode::EndLevel(bool bWin)
{
	//todo
		//몬스터 스폰 정지
		//플레이어 입력 정지
		//UI출력

	//종료 조건 추가 시 DietGameState 유효성 검사 추가 검토하기
	DietGameState->StopTimer();
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] 게임 종료 플레이어 %s"), bWin ? TEXT("승리") : TEXT("패배"));
}

void ADietGameMode::CommandSpawn()
{
}

void ADietGameMode::HandleWaveIncrease(int32 Wave)
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Wave Increased: %d"), Wave);
}

void ADietGameMode::HandleTimeUp()
{
	UE_LOG(LogTemp, Log, TEXT("[DietGameMode] Time Up!"));
	EndLevel(true);
}

ADietGameState* ADietGameMode::GetDietGameState() const
{
	if (DietGameState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DietGameMode] GameStateRef is null"));
		return nullptr;
	}
	return DietGameState;
}
