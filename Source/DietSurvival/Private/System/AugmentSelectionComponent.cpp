#include "System/AugmentSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"
#include "System/DataTableSubsystem.h"

#include "UI/TemplateAugmentSelectionBase.h"
#include "UI/AugmentSelectionWidget.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerStatComponent.h"

UAugmentSelectionComponent::UAugmentSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAugmentSelectionComponent::HandleLevelUp(int32 NewLevel)
{
	HandlePending();
}

void UAugmentSelectionComponent::TryBindToDelegate()
{
	APlayerController* PC = GetOwningController();
	CachedPS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	if (!CachedPS) { return; }

	CachedPS->OnLevelUp.AddDynamic(this, &UAugmentSelectionComponent::HandleLevelUp);
}

void UAugmentSelectionComponent::LoadCandidates()
{
	CachedCandidates = CachedPS->AugmentManager->SelectRandomAugments();
}

void UAugmentSelectionComponent::InitializeSelectionWidget()
{
	ActiveWidgetInstance->InitializeCards(CachedCandidates);
	ActiveWidgetInstance->OnAugmentChosen.AddDynamic(this, &UAugmentSelectionComponent::HandleAugmentChosen);
 	if (UAugmentSelectionWidget* Instance = Cast<UAugmentSelectionWidget>(ActiveWidgetInstance))
	{
		Instance->OnRerollPressed.AddDynamic(this, &UAugmentSelectionComponent::HandleRerollPressed);
	}
}

void UAugmentSelectionComponent::ApplyAugment(FName AugmentFName, int32 Level)
{
	if (UDataTableSubsystem* Subsystem = UDataTableSubsystem::Get(this))
	{
		// UPlayerStatComponent::UpgradeStat에 필요한 파라미터 준비
		EPlayerStatType StatType = Subsystem->GetAugmentStatType(AugmentFName);
		float StatAmount = 0.f;
		if (Level >= 0)
		{
			StatAmount = Subsystem->GetAugmentDelta(AugmentFName, Level);
		}

		APlayerController* PC = GetOwningController();
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn()))
		{
			Player->StatComponent->UpgradeStat(StatType, StatAmount);
			if (CachedPS)
			{
				CachedPS->AugmentManager->AugmentLevelUp(AugmentFName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Possess중인 Pawn이 APlayerCharacter가 아님."));
		}
	}
}

void UAugmentSelectionComponent::HandleRerollPressed()
{
	if (ActiveWidgetInstance)
	{
		// 증강 선택 풀 삭제
		ActiveWidgetInstance->ClearContainer();

		// 새로 랜덤한 증강 최대 3개 뽑기
		CachedCandidates.Reset();
		LoadCandidates();
		if (CachedCandidates.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("받아온 증강 없음."));
			return;
		}
		for (const auto& [Name, Level] : CachedCandidates)
		{
			UE_LOG(LogTemp, Warning, TEXT("증강: %s"), *Name.ToString());
		}

		// 새로운 증강 풀로 컨테이너 초기화
		ActiveWidgetInstance->InitializeCards(CachedCandidates);
	}
}
