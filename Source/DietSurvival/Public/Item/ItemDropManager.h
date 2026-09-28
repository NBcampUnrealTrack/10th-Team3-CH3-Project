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
	// EnemyBase::OnEnemyDeath 바인딩(델리게이트)
	UFUNCTION()
	void RequestDrop(FVector SpawnLocation, int32 InExp);

private:
	AActor* SpawnItemFromPool(UClass* ItemClass, FVector BaseLocation, const FString& ItemName);

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
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues);
	void Init();

	FDelegateHandle WorldInitDelegateHandle;
	bool bPoolsInitialized = false;

};
