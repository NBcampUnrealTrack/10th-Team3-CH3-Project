#include "System/AugmentSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"

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
}

void UAugmentSelectionComponent::TryBindToLevelUp()
{
	APlayerController* PC = GetOwningController();
	ADietPlayerState* PS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	if (!PS) { return; }

	PS->OnLevelUp.AddDynamic(this, &UAugmentSelectionComponent::HandleLevelUp);
}

void UAugmentSelectionComponent::StartSelection()
{
	APlayerController* PC = GetOwningController();
	if (!PC || bIsSelecting) { return; }

	bIsSelecting = true;

	ADietPlayerState* PS = PC->GetPlayerState<ADietPlayerState>();
	TArray<TTuple<FName, int32>> Candidates = PS->AugmentManager->SelectRandomAugments();
}

void UAugmentSelectionComponent::FinishSelection()
{
}
