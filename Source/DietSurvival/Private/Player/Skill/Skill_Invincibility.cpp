#include "Player/Skill/Skill_Invincibility.h"
#include "Player/PlayerCharacter.h"

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
	// 레벨업할 때마다 무적 지속시간이 늘어남
	InvincibilityDuration += DurationIncreasePerLevel;
}
