// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPoolObjectComponent;
class UProjectileMovementComponent;

UCLASS()
class DIETSURVIVAL_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor) override;
	virtual void ActivateItem(APawn* Activator) override;
	virtual FName GetItemType() override;

protected:

	// 충돌 감지용 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> CollisionSphere;

	// 자석 감지 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> MagnetSphere;

	// 추적용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY()
	TObjectPtr<UPoolObjectComponent> PoolObjectComponent;



private:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleMagnetBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
