#include "UI/MinimapSubsystem.h"

void UMinimapSubsystem::Register(UMinimapTrackComponent* Component)
{
	Tracked.AddUnique(Component);
}

void UMinimapSubsystem::Unregister(UMinimapTrackComponent* Component)
{
	Tracked.RemoveSwap(Component);
}
