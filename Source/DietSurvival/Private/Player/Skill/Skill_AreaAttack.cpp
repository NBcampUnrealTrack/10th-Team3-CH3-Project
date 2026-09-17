#include "Player/Skill/Skill_AreaAttack.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerStatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"


USkill_AreaAttack::USkill_AreaAttack()
{
	SkillName = TEXT("광역 스킬");
}

void USkill_AreaAttack::Activate()
{
	APlayerCharacter* Owner = OwnerCharacter.Get();
	if (!Owner)
	{
		return;
	}

	const FVector Center = Owner->GetActorLocation();

	// 이 범위 안에 있는 Pawn 오브젝트 타입 액터들을 전부 수집
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(
		Owner,
		Center,
		Radius,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		nullptr,
		IgnoreActors,
		OverlappedActors
	);

	// 기본 공격력(StatComponent)에 광역기 배율을 곱해서 최종 데미지 계산
	float BaseDamage = 10.f;
	if (const UPlayerStatComponent* Stat = Owner->FindComponentByClass<UPlayerStatComponent>())
	{
		BaseDamage = Stat->GetAttackPower();
	}
	const float FinalDamage = BaseDamage * DamageMultiplier;

	for (AActor* Target : OverlappedActors)
	{
		// AttackComponent와 동일하게 "Enemy" 태그로 적만 걸러냄
		if (Target && Target->ActorHasTag(TEXT("Enemy")))
		{
			UGameplayStatics::ApplyDamage(Target, FinalDamage, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());

			UE_LOG(LogTemp, Log, TEXT("[Skill_AreaAttack] %s에게 %.1f 데미지 적용"), *Target->GetName(), FinalDamage);
		}
	}

#if ENABLE_DRAW_DEBUG
	// 범위 확인용 디버그 구체
	DrawDebugSphere(Owner->GetWorld(), Center, Radius, 24, FColor::Orange, false, 1.f);
#endif
}

void USkill_AreaAttack::OnLevelUp()
{
	Super::OnLevelUp();

	DamageMultiplier += DamageMultiplierIncreasePerLevel;
	Radius += RadiusIncreasePerLevel;
	Cooldown -= CooldownDecreasePerLevel;
}
