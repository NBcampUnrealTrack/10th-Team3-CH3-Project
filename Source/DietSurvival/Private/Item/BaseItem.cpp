// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BaseItem.h"
#include "Pool/PoolObjectComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(50.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	MagnetSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MagnetSphere"));
	MagnetSphere->SetupAttachment(RootComponent);
	MagnetSphere->SetSphereRadius(200.0f);
	MagnetSphere->SetMobility(EComponentMobility::Movable);
	MagnetSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MagnetSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MagnetSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 2000.0f;
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 800.0f;

	PoolObjectComponent = CreateDefaultSubobject<UPoolObjectComponent>("PoolObject");

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::HandleBeginOverlap);

	MagnetSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::HandleMagnetBeginOverlap);
}

void ABaseItem::OnItemOverlap(AActor* OverlapActor)
{
	APawn* Player = Cast<APlayerCharacter>(OverlapActor);
	if (Player == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[BaseItem::OnItemOverlap] On item overlap by Player"));

	ActivateItem(Player);

	// 호밍 종료
	ProjectileMovement->Deactivate();
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->HomingTargetComponent = nullptr;

	if (PoolObjectComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("[BaseItem::OnItemOverlap] Item Return to pool"));
		PoolObjectComponent->ReturnToPool();
	}
	
}

void ABaseItem::OnItemEndOverlap(AActor* OverlapActor)
{
}

void ABaseItem::ActivateItem(APawn* Activator)
{
	UE_LOG(LogTemp, Log, TEXT("[BaseItem::ActivateItem] call function"));
}

FName ABaseItem::GetItemType()
{
	return FName("BaseItem");
}

void ABaseItem::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}
	OnItemOverlap(OtherActor);
}

void ABaseItem::HandleMagnetBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	// 호밍 시작
	ProjectileMovement->HomingTargetComponent = Player->GetRootComponent();
	ProjectileMovement->Activate();
}
