#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class DIETSURVIVAL_API ABossProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossProjectile();

	void FireAt(FVector TargetLocation);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Explode();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Explosion")
	float ExplosionRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Explosion")
	float ExplosionDelay = 1.0f;   // 착지 후 터지기까지의 딜레이

protected:
	UPROPERTY(VisibleAnywhere, Category = "Projectile|Collision")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Projectile|Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Projectile|Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Effect")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Effect")
	TObjectPtr<USoundBase> ExplosionSound;

private:
	FTimerHandle ExplosionTimerHandle;
	bool bHasLanded = false;
	bool bHasExploded = false;


};
