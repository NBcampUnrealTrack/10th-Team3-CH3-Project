#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DietPlayerState.generated.h"

class UAugmentManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpChangedSignature, int32, CurrentExp, int32, MaxExp);

UCLASS()
class DIETSURVIVAL_API ADietPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADietPlayerState();

	// 캐릭터가 Amount만큼의 경험치를 획득
	void GainExp(int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Exp")
	FOnLevelUpSignature OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Exp")
	FOnExpChangedSignature OnExpChanged;

protected:
	void LevelUp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diet|Augment")
	TObjectPtr<UAugmentManagerComponent> AugmentManager;

	int32 Exp = 0;

	int32 MaxExp = 10;

	int32 Level = 1;
};
