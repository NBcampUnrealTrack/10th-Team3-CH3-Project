#include "System/SkillSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/SkillManagerComponent.h"

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
	CachedPS->SkillManager->SkillLevelUp(ChosenSkillFName);
}

void USkillSelectionComponent::TryBindToSkillUp()
{
	APlayerController* PC = GetOwningController();
	CachedPS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	CachedPS->OnSkillUp.AddDynamic(this, &USkillSelectionComponent::HandleSkillUp);
}

void USkillSelectionComponent::StartSelection()
{
	bIsSelecting = true;
	CachedCandidates.Reset();
	CachedCandidates = CachedPS->SkillManager->GetSkillList();

	UE_LOG(LogTemp, Warning, TEXT("받아온 스킬 목록"));
	for (const auto& [Name, Level] : CachedCandidates)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, Current level: %d"), *Name.ToString(), Level);
	}

	FinishSelection();
}

void USkillSelectionComponent::FinishSelection()
{
	bIsSelecting = false;
}
