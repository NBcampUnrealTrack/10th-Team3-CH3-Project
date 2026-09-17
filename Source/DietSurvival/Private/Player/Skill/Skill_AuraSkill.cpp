#include "Player/Skill/Skill_AuraSkill.h"
#include "Player/Skill/AuraOrbActor.h"
#include "Player/PlayerCharacter.h"

USkill_AuraSkill::USkill_AuraSkill()
{
	SkillName = TEXT("오라 스킬");
}

void USkill_AuraSkill::OnAcquired(const FSkillDeltaRow& DeltaRow)
{
	if (DeltaRow.Row.IsValidIndex(0))
	{
		Damage = DeltaRow.Row[0];
	}

	if (DeltaRow.Row.IsValidIndex(1))
	{
		for (int i = 0; i < DeltaRow.Row[1]; i++)
		{
			SpawnOrb();
		}
	}

	RedistributeOrbAngles();
	UpdateAllOrbDamage();
}

void USkill_AuraSkill::OnLevelUp(const FSkillDeltaRow& DeltaRow)
{
	Super::OnLevelUp(DeltaRow);

	if (DeltaRow.Row.IsValidIndex(0))
	{
		Damage += DeltaRow.Row[0];
	}

	// Row[1]이 있는 레벨에서만 구체 추가 (없으면 스폰 안 함)
	if (DeltaRow.Row.IsValidIndex(1))
	{
		const int32 OrbsToAdd = FMath::RoundToInt(DeltaRow.Row[1]);
		for (int32 i = 0; i < OrbsToAdd; ++i)
		{
			SpawnOrb();
		}
		if (OrbsToAdd > 0)
		{
			RedistributeOrbAngles();
		}
	}

	UpdateAllOrbDamage();
}

void USkill_AuraSkill::SpawnOrb()
{
	APlayerCharacter* Owner = OwnerCharacter.Get();
	if (!Owner || !OrbActorClass)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;

	AAuraOrbActor* NewOrb = World->SpawnActor<AAuraOrbActor>(
		OrbActorClass, Owner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

	if (NewOrb)
	{
		// 각도는 RedistributeOrbAngles가 일괄로 다시 정해주므로 여기선 0으로 초기화만 해둠
		NewOrb->InitializeOrb(Owner, OrbitRadius, OrbitSpeed, 0.f);
		SpawnedOrbs.Add(NewOrb);
	}
}

void USkill_AuraSkill::RedistributeOrbAngles()
{
	const int32 OrbCount = SpawnedOrbs.Num();
	if (OrbCount == 0)
	{
		return;
	}

	// 오브 개수만큼 360도를 균등하게 나눠서 각자 다른 시작 각도를 줌
	const float AngleStep = 360.f / OrbCount;
	for (int32 i = 0; i < OrbCount; ++i)
	{
		if (AAuraOrbActor* Orb = SpawnedOrbs[i])
		{
			Orb->SetBaseAngleOffset(AngleStep * i);
		}
	}
}

void USkill_AuraSkill::UpdateAllOrbDamage()
{
	for (AAuraOrbActor* Orb : SpawnedOrbs)
	{
		if (Orb)
		{
			Orb->SetDamage(Damage);
		}
	}
}
