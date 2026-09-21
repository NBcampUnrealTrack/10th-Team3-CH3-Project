// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DietBossEnemy.h"
#include "Boss/BossProjectile.h"
#include "Enemy/Component/HealthComponent.h"

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

void ADietBossEnemy::HandleDeath()
{
	if (bIsDead) return;

	Super::HandleDeath();
	OnBossDeath.Broadcast();
	Destroy();
}
