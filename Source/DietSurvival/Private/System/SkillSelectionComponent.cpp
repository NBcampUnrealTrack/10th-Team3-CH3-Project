#include "System/SkillSelectionComponent.h"
#include "System/DietPlayerState.h"
#include "System/SkillManagerComponent.h"

#include "UI/TemplateAugmentSelectionBase.h"

#include "Player/PlayerCharacter.h"
#include "Player/Skill/SkillComponent.h"

USkillSelectionComponent::USkillSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillSelectionComponent::HandleSkillUp()
{
	HandlePending();
}

void USkillSelectionComponent::TryBindToDelegate()
{
	APlayerController* PC = GetOwningController();
	CachedPS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	if (!CachedPS) { return; }

	CachedPS->OnSkillUp.AddDynamic(this, &USkillSelectionComponent::HandleSkillUp);
}

void USkillSelectionComponent::LoadCandidates()
{
	CachedCandidates = CachedPS->SkillManager->GetSkillList();
}

void USkillSelectionComponent::InitializeSelectionWidget()
{
	ActiveWidgetInstance->InitializeCards(CachedCandidates);
	ActiveWidgetInstance->OnAugmentChosen.AddDynamic(this, &USkillSelectionComponent::HandleAugmentChosen);
}

void USkillSelectionComponent::ApplyAugment(FName AugmentFName, int32 Level) {
	// 스킬 컴포넌트에 스킬 증강 적용
	APlayerController* PC = GetOwningController();
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn()))
	{
		Player->SkillComponent->AcquireOrUpgradeSkill(AugmentFName, Level);
		if (CachedPS)
		{
			CachedPS->SkillManager->SkillLevelUp(AugmentFName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Possess중인 Pawn이 APlayerCharacter가 아님."));
	}
}
