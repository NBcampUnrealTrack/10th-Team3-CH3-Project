#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/MinimapStyleData.h"
#include "MinimapTrackComponent.generated.h"

UCLASS(ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class DIETSURVIVAL_API UMinimapTrackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void SetTracked(bool bInTracked) { bTracked = bInTracked; }

	bool IsTracked() const { return bTracked; }
	EMinimapIconType GetIconType() const { return IconType; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	EMinimapIconType IconType = EMinimapIconType::Enemy;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	bool bTracked = true;
};
