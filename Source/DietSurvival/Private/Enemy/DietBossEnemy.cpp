// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DietBossEnemy.h"
#include "Boss/BossProjectile.h"
#include "Enemy/Component/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADietBossEnemy::ADietBossEnemy()
{
	HealthComponent->Initailize(BossMaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("[BossEnemy::BeginPlay] (test)fire !"));
}

void ADietBossEnemy::BeginPlay()
{
	//Super::BeginPlay();
	//FireProjectileAt(FVector(1000.0f, 1000.0f, 0.0f));
}

void ADietBossEnemy::FireProjectileAt(FVector TargetLocation)
{
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[BossEnemy::FireProjectileAt] ProjectileClass is nullptr"));
		return;
	}

	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);   // 약간 위에서 발사 (임시)
	ABossProjectile* Projectile = GetWorld()->SpawnActor<ABossProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (Projectile != nullptr)
	{
		Projectile->SetOwner(this);
		Projectile->FireAt(TargetLocation);
	}
}

void ADietBossEnemy::ChargeAt(FVector TargetLocation)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	bIsCharging = true;

	LaunchCharacter(Direction * ChargeSpeed, true, false);

	GetWorldTimerManager().SetTimer(ChargeTimeoutHandle, this, &ADietBossEnemy::EndCharge, MaxChargeDuration, false);

}

bool ADietBossEnemy::IsPlayerInChargeRange() const
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn == nullptr)
	{
		return false;
	}

	return FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) <= ChargeRange;
}

void ADietBossEnemy::HandleDeath()
{
	if (bIsDead) return;

	Super::HandleDeath();
	OnBossDeath.Broadcast();
	Destroy();
}

void ADietBossEnemy::OnCapsuleOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	Super::OnCapsuleOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!bIsCharging)
	{
		return;
	}

	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	// 넉백 처리
	FVector KnockbackDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	PlayerCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);

	// 돌진 종료
	EndCharge();
}

void ADietBossEnemy::EndCharge()
{
	if (!bIsCharging)
	{
		return;
	}
	bIsCharging = false;

	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().ClearTimer(ChargeTimeoutHandle);
}
