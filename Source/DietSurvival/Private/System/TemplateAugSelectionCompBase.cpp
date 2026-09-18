#include "System/TemplateAugSelectionCompBase.h"
#include "System/DietPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "UI/TemplateAugmentSelectionBase.h"

UTemplateAugSelectionCompBase::UTemplateAugSelectionCompBase()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTemplateAugSelectionCompBase::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay시점에 PlayerState가 nullptr일 수도 있으니 바인드를 한 틱 미룬다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTemplateAugSelectionCompBase::TryBindToDelegate);
}

void UTemplateAugSelectionCompBase::EndPlay(const EEndPlayReason::Type Reason)
{
	if (CachedPS)
	{
		CachedPS->OnLevelUp.RemoveAll(this);
		CachedPS = nullptr;
	}

	Super::EndPlay(Reason);
}

void UTemplateAugSelectionCompBase::HandleAugmentChosen(FName ChosenAugmentFName) {
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
	ApplyAugment(ChosenAugmentFName, AugmentLevel);

	CachedCandidates.Reset();
	FinishSelection();
}

void UTemplateAugSelectionCompBase::TryBindToDelegate()
{
}

void UTemplateAugSelectionCompBase::StartSelection()
{
	APlayerController* PC = GetOwningController();
	if (!PC || bIsSelecting) { return; }

	bIsSelecting = true;

	// 증강 선택지 가져오기
	CachedCandidates.Reset();
	LoadCandidates();
	if (CachedCandidates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("받아온 증강 없음."));
		return;
	}
	for (const auto& [Name, Level] : CachedCandidates)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, Current level: %d"), *Name.ToString(), Level);
	}

	// 위젯 관련
	ActiveWidgetInstance = CreateWidget<UTemplateAugmentSelectionBase>(PC, SelectionWidgetClass);
	InitializeSelectionWidget();
	ActiveWidgetInstance->AddToViewport();

	// 정지, 입력모드
	PC->SetPause(true);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void UTemplateAugSelectionCompBase::FinishSelection()
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

	if (PendingAugmentCount > 0)
	{
		PendingAugmentCount--;
		StartSelection();
	}
}

void UTemplateAugSelectionCompBase::HandlePending()
{
	// 한 번에 스킬 아이템을 2개 이상 획득해 OnSkillUp이 여러 번 Broadcast 될 수도 있음.
	PendingAugmentCount++;
	if (!bIsSelecting)
	{
		PendingAugmentCount--;
		StartSelection();
	}
}

void UTemplateAugSelectionCompBase::LoadCandidates() {

}

void UTemplateAugSelectionCompBase::InitializeSelectionWidget() {

}

void UTemplateAugSelectionCompBase::ApplyAugment(FName AugmentFName, int32 Level)
{
}
