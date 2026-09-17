#include "System/SkillSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/SkillManagerComponent.h"
#include "UI/SkillSelectionWidget.h"

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

	// 임시로 스킬 레벨 증가만.
	CachedPS->SkillManager->SkillLevelUp(ChosenSkillFName);

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
