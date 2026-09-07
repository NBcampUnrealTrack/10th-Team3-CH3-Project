#include "System/AugmentManagerComponent.h"

UAugmentManagerComponent::UAugmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAugmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAugmentManagerComponent::StartAugment()
{
}

TArray<FName> UAugmentManagerComponent::SelectRandomAugments()
{
	return TArray<FName>();
}
