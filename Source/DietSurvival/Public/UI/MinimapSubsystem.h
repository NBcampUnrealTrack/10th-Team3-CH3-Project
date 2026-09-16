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

	const TArray<TObjectPtr<UMinimapTrackComponent>>& GetTracked() const { return Tracked; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UMinimapTrackComponent>> Tracked;
};
