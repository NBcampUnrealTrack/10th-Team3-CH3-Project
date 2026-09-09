#include "System/AugmentSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"
#include "Augment/AugmentSelectionWidget.h"

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
	FinishSelection();
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

	// 랜덤한 증강 최대 3개 뽑기
	ADietPlayerState* PS = PC->GetPlayerState<ADietPlayerState>();
	TArray<TTuple<FName, int32>> Candidates = PS->AugmentManager->SelectRandomAugments();

	if (Candidates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("받아온 증강 없음."));
	}
	for (const auto& [Name, Level] : Candidates)
	{
		UE_LOG(LogTemp, Warning, TEXT("증강: %s"), *Name.ToString());
	}

	// 위젯 관련
	ActiveWidgetInstance = CreateWidget<UAugmentSelectionWidget>(PC, SelectionWidgetClass);
	ActiveWidgetInstance->InitializeCards(Candidates);
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
