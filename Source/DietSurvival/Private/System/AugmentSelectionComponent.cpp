#include "System/AugmentSelectionComponent.h"
#include "System/AugmentManagerComponent.h"
#include "System/DataTableSubsystem.h"
#include "System/DietPlayerState.h"

#include "Augment/AugmentSelectionWidget.h"

#include "Player/PlayerStatComponent.h"
#include "Player/PlayerCharacter.h"

UAugmentSelectionComponent::UAugmentSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAugmentSelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay시점에 PlayerState가 nullptr일 수도 있으니 바인드를 한 틱 미룬다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UAugmentSelectionComponent::TryBindToLevelUp);
}

void UAugmentSelectionComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (CachedPS)
	{
		CachedPS->OnLevelUp.RemoveDynamic(this, &UAugmentSelectionComponent::HandleLevelUp);
		CachedPS = nullptr;
	}

	Super::EndPlay(Reason);
}

void UAugmentSelectionComponent::HandleLevelUp(int32 NewLevel)
{
	// 한 번에 2렙업 이상 진행되어 OnLevelUp이 여러 번 Broadcast 될 수도 있음.
	PendingLevelUpCount++;
	if (!bIsSelecting)
	{
		PendingLevelUpCount--;
		StartSelection();
	}
}

void UAugmentSelectionComponent::HandleAugmentChosen(FName ChosenAugmentFName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s 증강 선택."), *ChosenAugmentFName.ToString());

	if (UDataTableSubsystem* Subsystem = UDataTableSubsystem::Get(this))
	{
		// 선택한 증강의 현재 레벨 파악
		int32 AugmentLevel = -1;
		for (const auto& [Name, Level] : CachedCandidates)
		{
			if (Name == ChosenAugmentFName)
			{
				AugmentLevel = Level;
				break;
			}
		}
		if (AugmentLevel == -1)
		{
			UE_LOG(LogTemp, Error, TEXT("큰일남. 증강 레벨을 알 수 없음."));
		}

		// UPlayerStatComponent::UpgradeStat에 필요한 파라미터 준비
		EPlayerStatType StatType = Subsystem->GetAugmentStatType(ChosenAugmentFName);
		float StatAmount = 0.f;
		if (AugmentLevel >= 0)
		{
			StatAmount = Subsystem->GetAugmentDelta(ChosenAugmentFName, AugmentLevel);
		}

		APlayerController* PC = GetOwningController();
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn()))
		{
			Player->StatComponent->UpgradeStat(StatType, StatAmount);
			if (CachedPS)
			{
				CachedPS->AugmentManager->AugmentLevelUp(ChosenAugmentFName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Possess중인 Pawn이 APlayerCharacter가 아님."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UAugmentSelectionComponent::HandleAugmentChosen - DataTableSubsystem을 가져올 수 없음"));
	}
	CachedCandidates.Reset();
	FinishSelection();
}

void UAugmentSelectionComponent::TryBindToLevelUp()
{
	APlayerController* PC = GetOwningController();
	CachedPS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	if (!CachedPS) { return; }

	CachedPS->OnLevelUp.AddDynamic(this, &UAugmentSelectionComponent::HandleLevelUp);
}

void UAugmentSelectionComponent::StartSelection()
{
	APlayerController* PC = GetOwningController();
	if (!PC || bIsSelecting) { return; }

	bIsSelecting = true;

	// 랜덤한 증강 최대 3개 뽑기
	CachedCandidates.Reset();
	ADietPlayerState* PS = PC->GetPlayerState<ADietPlayerState>();
	CachedCandidates = PS->AugmentManager->SelectRandomAugments();

	if (CachedCandidates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("받아온 증강 없음."));
	}
	for (const auto& [Name, Level] : CachedCandidates)
	{
		UE_LOG(LogTemp, Warning, TEXT("증강: %s"), *Name.ToString());
	}

	// 위젯 관련
	ActiveWidgetInstance = CreateWidget<UAugmentSelectionWidget>(PC, SelectionWidgetClass);
	ActiveWidgetInstance->InitializeCards(CachedCandidates);
	ActiveWidgetInstance->OnAugmentChosen.AddDynamic(this, &UAugmentSelectionComponent::HandleAugmentChosen);
	ActiveWidgetInstance->AddToViewport();

	// 정지, 입력모드
	PC->SetPause(true);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void UAugmentSelectionComponent::FinishSelection()
{
	APlayerController* PC = GetOwningController();
	if (!PC) { return; }

	if (ActiveWidgetInstance)
	{
		ActiveWidgetInstance->RemoveFromParent();
		ActiveWidgetInstance = nullptr;
	}

	PC->bShowMouseCursor = false;
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetPause(false);
	bIsSelecting = false;

	if (PendingLevelUpCount > 0)
	{
		PendingLevelUpCount--;
		StartSelection();
	}
}
