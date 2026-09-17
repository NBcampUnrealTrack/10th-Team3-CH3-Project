#include "UI/MinimapTrackComponent.h"
#include "UI/MinimapSubsystem.h"

void UMinimapTrackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UMinimapSubsystem* Subsystem = GetWorld()->GetSubsystem<UMinimapSubsystem>())
	{
		Subsystem->Register(this);
	}
}

void UMinimapTrackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UMinimapSubsystem* Subsystem = World->GetSubsystem<UMinimapSubsystem>())
		{
			Subsystem->Unregister(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}
