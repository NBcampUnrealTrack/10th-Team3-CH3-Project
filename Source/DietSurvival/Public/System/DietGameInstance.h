#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DietGameInstance.generated.h"

UCLASS()
class DIETSURVIVAL_API UDietGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static UDietGameInstance* Get(const UObject* WorldContext);

public:
	// lifecycle

	UDietGameInstance();
	virtual void Init() override;

private:
	// variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> AugmentDataTable;
};
