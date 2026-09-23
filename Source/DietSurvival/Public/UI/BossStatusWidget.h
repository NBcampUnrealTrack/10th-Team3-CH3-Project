#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossStatusWidget.generated.h"

class UProgressBar;
class ADietBossEnemy;

UCLASS()
class DIETSURVIVAL_API UBossStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetBoss(ADietBossEnemy* Boss);
	void SetRemainingTime(float RemainingSeconds);

protected:
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleBossHealthChanged(int32 CurrentHealth, int32 MaxHealth);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> TimeBar;

private:
	TWeakObjectPtr<ADietBossEnemy> CachedBoss;
	float TimeLimit = 0.f;
};
