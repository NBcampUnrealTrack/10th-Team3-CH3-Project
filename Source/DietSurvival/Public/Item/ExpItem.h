#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "ExpItem.generated.h"

UCLASS()
class DIETSURVIVAL_API AExpItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AExpItem();

public:
	virtual void ActivateItem(APawn* Activator) override;
	virtual FName GetItemType() override;

private:
	UPROPERTY(EditAnywhere, Category="Exp Amount")
	int32 ExpAmount;
};
