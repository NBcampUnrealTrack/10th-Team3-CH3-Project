#include "UI/MinimapSubsystem.h"

void UMinimapSubsystem::Register(UMinimapTrackComponent* Component)
{
	PendingRemove.Remove(Component);
	Tracked.Add(Component);
}

void UMinimapSubsystem::Unregister(UMinimapTrackComponent* Component)
{
	PendingRemove.Add(Component);
}

const TSet<TObjectPtr<UMinimapTrackComponent>>& UMinimapSubsystem::GetTracked()
{
	for (UMinimapTrackComponent* Component : PendingRemove)
	{
		Tracked.Remove(Component);
	}
	PendingRemove.Reset();
	return Tracked;
}
