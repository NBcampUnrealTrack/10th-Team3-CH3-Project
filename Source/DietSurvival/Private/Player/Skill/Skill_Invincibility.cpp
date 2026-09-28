#include "Player/Skill/Skill_Invincibility.h"
#include "Player/PlayerCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

		// 무적 이펙트 재생
		if (!SkillEffect) { return; }
		UNiagaraComponent* EffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SkillEffect,
			Owner->GetMesh(),
			TEXT("Root"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true, // bAutoDestroy 재생이 끝나면 종료
			false // bAutoActivate: 스폰 직후 바로 켜지는 걸 방지
		);

		if (EffectComp)
		{
			EffectComp->SetNiagaraVariableFloat(TEXT("User.EffectDuration"), InvincibilityDuration);
			EffectComp->Activate(true);
		}
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
