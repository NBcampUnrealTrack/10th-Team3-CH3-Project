#include "System/SkillSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/SkillManagerComponent.h"
#include "System/DataTableSubsystem.h"

#include "UI/SkillSelectionWidget.h"

#include "Player/PlayerCharacter.h"
#include "Player/Skill/SkillComponent.h"

USkillSelectionComponent::USkillSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USkillSelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay시점에 PlayerState가 nullptr일 수도 있으니 바인드를 한 틱 미룬다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USkillSelectionComponent::TryBindToSkillUp);
}

void USkillSelectionComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (CachedPS)
	{
		CachedPS->OnSkillUp.RemoveDynamic(this, &USkillSelectionComponent::HandleSkillUp);
		CachedPS = nullptr;
	}
	Super::EndPlay(Reason);
}

void USkillSelectionComponent::HandleSkillUp()
{
	// 한 번에 스킬 아이템을 2개 이상 획득해 OnSkillUp이 여러 번 Broadcast 될 수도 있음.
	PendingSkillUpCount++;
	if (!bIsSelecting)
	{
		PendingSkillUpCount--;
		StartSelection();
	}
}

void USkillSelectionComponent::HandleSkillChosen(FName ChosenSkillFName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s 스킬 선택."), *ChosenSkillFName.ToString());

	if (UDataTableSubsystem* Subsystem = UDataTableSubsystem::Get(this))
	{
		// 선택한 증강의 현재 레벨 파악
		int32 AugmentLevel = -1;
		for (const auto& [Name, Level] : CachedCandidates)
		{
			if (Name == ChosenSkillFName)
			{
				AugmentLevel = Level;
				break;
			}
		}
		if (AugmentLevel == -1)
		{
			UE_LOG(LogTemp, Error, TEXT("큰일남. 증강 레벨을 알 수 없음."));
		}

		// 스킬 컴포넌트에 스킬 증강 적용
		APlayerController* PC = GetOwningController();
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn()))
		{
			Player->SkillComponent->AcquireOrUpgradeSkill(Subsystem->GetSkillClass(ChosenSkillFName), ChosenSkillFName, AugmentLevel);
			if (CachedPS)
			{
				CachedPS->SkillManager->SkillLevelUp(ChosenSkillFName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Possess중인 Pawn이 APlayerCharacter가 아님."));
		}
	}
	
	CachedCandidates.Reset();
	FinishSelection();
}

void USkillSelectionComponent::TryBindToSkillUp()
{
	APlayerController* PC = GetOwningController();
	CachedPS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	CachedPS->OnSkillUp.AddDynamic(this, &USkillSelectionComponent::HandleSkillUp);
}

void USkillSelectionComponent::StartSelection()
{
	APlayerController* PC = GetOwningController();
	if (!PC || bIsSelecting) { return; }

	bIsSelecting = true;

	// 최대 레벨에 도달하지 않은 스킬 목록 가져오기
	CachedCandidates.Reset();
	CachedCandidates = CachedPS->SkillManager->GetSkillList();

	UE_LOG(LogTemp, Warning, TEXT("받아온 스킬 목록"));
	if (CachedCandidates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("받아온 스킬 목록 없음."));
		return;
	}
	for (const auto& [Name, Level] : CachedCandidates)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, Current level: %d"), *Name.ToString(), Level);
	}
	
	// 위젯 관련
	ActiveWidgetInstance = CreateWidget<USkillSelectionWidget>(PC, SelectionWidgetClass);
	ActiveWidgetInstance->InitializeCards(CachedCandidates);
	ActiveWidgetInstance->OnSkillChosen.AddDynamic(this, &USkillSelectionComponent::HandleSkillChosen);
	ActiveWidgetInstance->AddToViewport();

	// 정지, 입력모드
	PC->SetPause(true);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void USkillSelectionComponent::FinishSelection()
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

	if (PendingSkillUpCount > 0)
	{
		PendingSkillUpCount--;
		StartSelection();
	}
}
