// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DietBossEnemy.h"
#include "Boss/BossProjectile.h"
#include "Enemy/Component/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundBase.h"

ADietBossEnemy::ADietBossEnemy()
{
	HealthComponent->Initailize(BossMaxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("[BossEnemy::BeginPlay] (test)fire !"));
}

void ADietBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent != nullptr)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ADietBossEnemy::HandleHealthChanged);
	}

	if (SpawnSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, SpawnSound);
	}
}

void ADietBossEnemy::FireProjectileAt(FVector TargetLocation)
{
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[BossEnemy::FireProjectileAt] ProjectileClass is nullptr"));
		return;
	}

	if (ThrowSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, ThrowSound);
	}

	FaceTowardsPlayer();

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
	FaceTowardsPlayer();

	if (ChargeSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, ChargeSound);
	}

	UE_LOG(LogTemp, Log, TEXT("[ADietBossEnemy::ChargeAt] Called. Target: %s"), *TargetLocation.ToString());

	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	bIsCharging = true;

	LaunchCharacter(Direction * ChargeSpeed, true, false);

	GetWorldTimerManager().SetTimer(ChargeTimeoutHandle, this, &ADietBossEnemy::EndCharge, MaxChargeDuration, false);
	UE_LOG(LogTemp, Log, TEXT("[ADietBossEnemy::ChargeAt] LaunchCharacter velocity: %s"), *(Direction * ChargeSpeed).ToString());
}

bool ADietBossEnemy::IsPlayerInChargeRange() const
{
	DrawDebugCircle(GetWorld(), GetActorLocation(), ChargeRange, 32, FColor::Red, false, -1.0f, 0, 2.0f, FVector(1, 0, 0), FVector(0, 1, 0), false);
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

void ADietBossEnemy::HandleHealthChanged(int32 CurrentHealth, int32 MaxHealth)
{
	OnBossHealthChanged.Broadcast(CurrentHealth, MaxHealth);
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
	if (!OtherActor || !OtherActor->ActorHasTag("Player"))
	{
		return;   // 플레이어가 아니면 완전히 무시
	}

	UGameplayStatics::ApplyDamage(
		OtherActor,
		BossPowerAttack,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

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
	UE_LOG(LogTemp, Log, TEXT("[ADietBossEnemy::EndCharge] Called. Was charging: %s"), bIsCharging ? TEXT("true") : TEXT("false"));
	if (!bIsCharging)
	{
		return;
	}
	bIsCharging = false;

	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().ClearTimer(ChargeTimeoutHandle);
}

void ADietBossEnemy::FaceTowardsPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn == nullptr)
	{
		return;
	}

	FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.0f;

	FRotator TargetRotation = ToPlayer.Rotation();
	SetActorRotation(TargetRotation);  
}

void ADietBossEnemy::PlayThrowSound()
{
	if (ThrowSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, ThrowSound);
	}
}
