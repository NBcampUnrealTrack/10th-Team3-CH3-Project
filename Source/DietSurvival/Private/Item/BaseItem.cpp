// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BaseItem.h"
#include "Components/SphereComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(50.0f);

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::HandleBeginOverlap);
	
}

void ABaseItem::OnItemOverlap(AActor* OverlapActor)
{
	APawn* Player = Cast<APawn>(OverlapActor);
	if (Player == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[BaseItem::OnItemOverlap] On item overlap by Player"));

	ActivateItem(Player);
	Destroy();
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
