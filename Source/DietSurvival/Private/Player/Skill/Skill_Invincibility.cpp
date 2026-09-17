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

void USkill_Invincibility::OnLevelUp()
{
	Super::OnLevelUp();
	// 레벨업할 때마다 무적 지속시간이 늘어남
	InvincibilityDuration += DurationIncreasePerLevel;
}
