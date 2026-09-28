#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MinimapSubsystem.generated.h"

class UMinimapTrackComponent;

UCLASS()
class DIETSURVIVAL_API UMinimapSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void Register(UMinimapTrackComponent* Component);
	void Unregister(UMinimapTrackComponent* Component);

	const TSet<TObjectPtr<UMinimapTrackComponent>>& GetTracked();

private:
	UPROPERTY()
	TSet<TObjectPtr<UMinimapTrackComponent>> Tracked;

	UPROPERTY()
	TSet<TObjectPtr<UMinimapTrackComponent>> PendingRemove;
};
