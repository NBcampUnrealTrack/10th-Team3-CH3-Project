#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraOrbActor.generated.h"

class UStaticMeshComponent;
class APlayerCharacter;


 // 오라 스킬이 스폰하는 실제 구체 액터.
 // 플레이어 주변을 계속 회전하며, 자기 자신의 콜리전으로 적과 겹치면 직접 데미지를 줌.
UCLASS()
class DIETSURVIVAL_API AAuraOrbActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraOrbActor();

	virtual void Tick(float DeltaTime) override;

	// 스킬이 스폰 직후 한 번 호출해서 기본 설정을 넣어줌 
	void InitializeOrb(APlayerCharacter* InOwner, float InOrbitRadius, float InOrbitSpeed, float InBaseAngleOffset);

	// 오브 개수가 바뀌어 재배치할 때 스킬이 호출 (균등 간격 재계산용) 
	void SetBaseAngleOffset(float NewOffset) { BaseAngleOffset = NewOffset; }

	// 레벨업으로 데미지 증가할 때
	void SetDamage(float NewDamage) { Damage = NewDamage; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> AttackSound;

	UFUNCTION()
	void OnOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 이 오브를 소유한 플레이어. 이 위치를 중심으로 궤도를 돎
	TWeakObjectPtr<APlayerCharacter> OwnerCharacter;

	float OrbitRadius = 200.f;
	float OrbitSpeed = 90.f;      // 초당 회전 각도(도)
	float BaseAngleOffset = 0.f;  // 여러 오브가 겹치지 않고 균등하게 퍼지도록 각자 다른 시작 각도를 가짐
	float Damage = 0.f;
};
