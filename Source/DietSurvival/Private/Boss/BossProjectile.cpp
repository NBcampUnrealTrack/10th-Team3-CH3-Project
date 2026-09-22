// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossProjectile::ABossProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(20.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionSphere->SetNotifyRigidBodyCollision(true);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ABossProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABossProjectile::HandleHit);
}

void ABossProjectile::FireAt(FVector TargetLocation)
{
	if (GetOwner() != nullptr)
	{
		CollisionSphere->IgnoreActorWhenMoving(GetOwner(), true);
	}

	ProjectileMovement->ProjectileGravityScale = 1.0f;

	FVector LaunchVelocity;
	bool bSuccess = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		LaunchVelocity,
		GetActorLocation(),
		TargetLocation,
		0.0f,     // OverrideGravityZ (0 = 월드 기본 중력 사용)
		0.5f      // ArcParam (포물선 높이 정도, 임시값)
	);

	if (bSuccess)
	{
		ProjectileMovement->Velocity = LaunchVelocity;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[BossProjectile] Failed to calculate launch velocity"));
	}
}


void ABossProjectile::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ABossProjectile::Explode()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 16, FColor::Red, false, 0.5f);

	UGameplayStatics::ApplyRadialDamage(
		this,
		DamageAmount,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoredActors,
		this,
		nullptr
	);

	Destroy();
}

