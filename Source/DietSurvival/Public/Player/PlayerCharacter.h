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
class UAttackComponent;
class USkillComponent;

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
	bool IsInvincible() const { return bIsHitInvincible || bIsSkillInvincible; }

	// 무적 상태가 바뀌었을 때 브로드캐스트되는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Invincibility")
	FOnInvincibilityChanged OnInvincibilityChanged;

	// 일정 시간 동안 무적 상태로 만드는 함수. 스킬에서 호출됨
	UFUNCTION(BlueprintCallable, Category = "Invincibility")
	void ActivateTemporaryInvincibility(float Duration);

protected:
	// 피격당했을 때 부여되는 무적 시간 (초) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Invincibility", meta = (AllowPrivateAccess = "true"))
	float HitInvincibilityDuration = 0.1f;

	bool bIsHitInvincible = false;
	bool bIsSkillInvincible = false;

	FTimerHandle HitInvincibilityTimerHandle;
	FTimerHandle SkillInvincibilityTimerHandle;

	// 피격 시 내부적으로 호출 
	void StartHitInvincibility();
	void EndHitInvincibility();
	// 스킬 무적 끝날 때 내부적으로 호출
	void EndSkillInvincibility();

public:
	//---------- 카메라 컴포넌트 ----------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	//--------- 컴포넌트들 ---------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerStatComponent> StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttackComponent> AttackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillComponent> SkillComponent;
	

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CycleSkillSlotAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UseSkillAction;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Pause();
	void Reload();
	void OnCycleSkillSlot(const FInputActionValue& Value);
	void OnUseSkillInput(const FInputActionValue& Value);

	//StatComponent의 OnFullnessChanged 구독용
	UFUNCTION()
	void HandleFullnessChanged(float NewFullness, float MaxFullnessValue);

	//StatComponent의 OnMoveSpeedChanged 구독용
	UFUNCTION()
	void HandleMoveSpeedChanged(float NewEffectiveSpeed);
};
