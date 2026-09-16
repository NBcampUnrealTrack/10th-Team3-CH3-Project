#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "DigestiveItem.generated.h"


UCLASS()
class DIETSURVIVAL_API ADigestiveItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ADigestiveItem();

public:
	virtual void ActivateItem(APawn* Activator) override;
	virtual FName GetItemType() override;

private:
	UPROPERTY(EditAnywhere, Category="Digestive Amount")
	float DigestiveAmount;
};
