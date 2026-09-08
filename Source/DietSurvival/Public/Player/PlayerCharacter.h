#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerStatComponent;

// 무적 상태 변화 시 브로드캐스트 (UI에서 구독)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvincibilityChanged, bool, bIsNowInvincible);


UCLASS()
class DIETSURVIVAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

   
public:
	// ------무적 시스템---------
	//지금 무적인가?
	UFUNCTION(BlueprintCallable, Category = "Invincibility")
	bool IsInvincible() const { return bIsHitInvincible; }

	UPROPERTY(BlueprintAssignable, Category = "Invincibility")
	FOnInvincibilityChanged OnInvincibilityChanged;

protected:
	// 피격당했을 때 부여되는 무적 시간 (초) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Invincibility", meta = (AllowPrivateAccess = "true"))
	float HitInvincibilityDuration = 0.1f;

	bool bIsHitInvincible = false;

	FTimerHandle HitInvincibilityTimerHandle;

	// 피격 시 내부적으로 호출 
	void StartHitInvincibility();
	void EndHitInvincibility();

public:
	//---------- 카메라 컴포넌트 ----------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	//--------- 스탯 컴포넌트 ---------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerStatComponent> StatComponent;

protected:
	//---------- 입력 관련 ----------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


};
