#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class DIETSURVIVAL_API IItemInterface
{
	GENERATED_BODY()
	
public:
	virtual void OnItemOverlap(AActor* OverlapActor) = 0;
	virtual void OnItemEndOverlap(AActor* OverlapActor) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() = 0;
};
