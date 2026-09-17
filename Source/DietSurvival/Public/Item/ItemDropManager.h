#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemDropManager.generated.h"

class APoolManager;
class AExpItem;
class ADigestiveItem;
class ASkillAugmentItem;

UCLASS()
class DIETSURVIVAL_API UItemDropManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UItemDropManager* Get(const UObject* WorldContext);

public:
	// lifecycle
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	// functions
	void Init();
	void RequestDrop(FVector SpawnLocation);

private:
	void SpawnItemFromPool(UClass* ItemClass, FVector BaseLocation, const FString& ItemName);

private:
	// variables
	UPROPERTY()
	TObjectPtr<APoolManager> CachedPoolManager;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AExpItem> ExpItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<ADigestiveItem> DigestiveItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<ASkillAugmentItem> SkillAugmentItemClass;

private:
	bool bPoolsInitialized = false;

};
