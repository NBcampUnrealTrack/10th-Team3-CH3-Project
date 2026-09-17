#include "Player/Skill/ActiveSkillBase.h"
#include "Player/PlayerCharacter.h"

bool UActiveSkillBase::TryActivate()
{
	// OwnerCharacter가 유효하지 않거나 쿨타임 중이면 발동 실패
	if (!OwnerCharacter.IsValid() || IsOnCooldown())
	{
		return false;
	}

	// 마지막 발동 시간을 갱신
	if (const UWorld* World = OwnerCharacter->GetWorld())
	{
		LastActivationWorldTime = World->GetTimeSeconds();
	}

	Activate();
	return true;
}

bool UActiveSkillBase::IsOnCooldown() const
{
	return GetRemainingCooldown() > 0.f;
}

float UActiveSkillBase::GetRemainingCooldown() const
{
	if (!OwnerCharacter.IsValid())
	{
		return 0.f;
	}

	// 남은 쿨타임 계산 : 쿨타임 - (현재 월드 시간 - 마지막 발동 시간)
	if (const UWorld* World = OwnerCharacter->GetWorld())
	{
		const float Elapsed = World->GetTimeSeconds() - LastActivationWorldTime;
		return FMath::Max(Cooldown - Elapsed, 0.f);
	}

	return 0.f;
}
