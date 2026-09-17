#include "Player/Skill/Skill_Invincibility.h"
#include "Player/PlayerCharacter.h"

USkill_Invincibility::USkill_Invincibility()
{
	SkillName = TEXT("무적 스킬");
}

void USkill_Invincibility::Activate()
{
	// PlayerCharacter가 이미 갖고 있는 무적 API를 그대로 호출.
	if (APlayerCharacter* Owner = OwnerCharacter.Get())
	{
		Owner->ActivateTemporaryInvincibility(InvincibilityDuration);
	}
}

void USkill_Invincibility::OnAcquired(const FSkillDeltaRow& DeltaRow)
{
	// 최초 획득 시 증강값이 곧 무적 지속 시간
	if (DeltaRow.Row.IsValidIndex(0))
	{
		InvincibilityDuration = DeltaRow.Row[0];
	}
}

void USkill_Invincibility::OnLevelUp(const FSkillDeltaRow& DeltaRow)
{
	Super::OnLevelUp(DeltaRow);

	if (DeltaRow.Row.IsValidIndex(0))
	{
		InvincibilityDuration += DeltaRow.Row[0];
	}
}
