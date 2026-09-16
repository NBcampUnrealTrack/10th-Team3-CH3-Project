#include "UI/MinimapTrackComponent.h"
#include "UI/MinimapSubsystem.h"
#include "Pool/PoolObjectComponent.h"

void UMinimapTrackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UMinimapSubsystem* Subsystem = GetWorld()->GetSubsystem<UMinimapSubsystem>())
	{
		Subsystem->Register(this);
	}

	PoolObject = GetOwner()->FindComponentByClass<UPoolObjectComponent>();
	if (PoolObject)
	{
		PoolObject->OnPoolActiveChanged.AddDynamic(this, &UMinimapTrackComponent::SetTracked);
	}
}

void UMinimapTrackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PoolObject)
	{
		PoolObject->OnPoolActiveChanged.RemoveDynamic(this, &UMinimapTrackComponent::SetTracked);
	}

	if (UWorld* World = GetWorld())
	{
		if (UMinimapSubsystem* Subsystem = World->GetSubsystem<UMinimapSubsystem>())
		{
			Subsystem->Unregister(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}
