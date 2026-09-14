#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "System/DietGameState.h"
#include "UI/MainHUD.h"

 #include "Player/PlayerStatComponent.h"
 #include "Player/AttackComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//---------- 카메라 세팅 ----------
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;         
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 60.f); 
	CameraBoom->bDoCollisionTest = true;         // 벽에 카메라가 파묻히지 않도록 충돌 검사 켬
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 15.f;         
	CameraBoom->bUsePawnControlRotation = true;  // 붐이 마우스 회전을 그대로 따라감

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{ 
		MoveComp->bOrientRotationToMovement = false; //마우스 방향으로만 회전.
	}

	Tags.Add(FName("Player"));

	//컴포넌트 생성
	StatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}

		if (StatComponent)
		{
			// 이벤트 구독 추가
			StatComponent->OnMoveSpeedChanged.AddDynamic(this, &APlayerCharacter::HandleMoveSpeedChanged);
			StatComponent->OnFullnessChanged.AddDynamic(this, &APlayerCharacter::HandleFullnessChanged);

			// 초기 이동 속도 적용
			if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
			{
				MoveComp->MaxWalkSpeed = StatComponent->GetEffectiveMoveSpeed();
			}
		}

		if (ADietGameState* GS = GetWorld()->GetGameState<ADietGameState>())
		{
			  GS->SetPlayerRef(this);
		}
	}

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (PauseAction)
		{
			EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerCharacter::Pause);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D InputVector = Value.Get<FVector2D>();

	if (InputVector.IsNearlyZero() || !Controller)
	{
		return;
	}

	const FRotator YawRotation(0.f, GetActorRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}

	//좌우
	AddControllerYawInput(LookAxisVector.X);

	//상하
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::Pause()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) { return; }

	if (AMainHUD* HUD = PC->GetHUD<AMainHUD>())
	{
		HUD->ShowPauseMenu();
	}
}

void APlayerCharacter::HandleFullnessChanged(float NewFullness, float MaxFullnessValue)
{
	// 포만감이 변경되어 임계값을 넘어가면 이동속도 변경
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		if (StatComponent)
		{
			MoveComp->MaxWalkSpeed = StatComponent->GetEffectiveMoveSpeed();
		}
	}
}

//변경된 이동속도를 캐릭터 무브먼트 컴포넌트에 적용
void APlayerCharacter::HandleMoveSpeedChanged(float NewEffectiveSpeed)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = NewEffectiveSpeed;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//무적이면 데미지 무시
	if (IsInvincible())
	{
		return 0.f;
	}

	// AActor 기본 구현: OnTakeAnyDamage 등 표준 델리게이트 브로드캐스트 후 DamageAmount를 그대로 반환
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent && ActualDamage > 0.f)
	{
		// 실제 스탯 변경은 StatComponent에만 위임 (적한테 맞으면 포만감 증가)
		StatComponent->ApplyDamage(ActualDamage);

		// 무적 시간 부여 
		StartHitInvincibility();
	}
	return ActualDamage;
}

//------------------- 무적 시스템 -------------------
void APlayerCharacter::StartHitInvincibility()
{
	bIsHitInvincible = true;
	OnInvincibilityChanged.Broadcast(true);

	GetWorldTimerManager().SetTimer(
		HitInvincibilityTimerHandle,
		this,
		&APlayerCharacter::EndHitInvincibility,
		HitInvincibilityDuration,
		false 
	);
}

void APlayerCharacter::EndHitInvincibility()
{
	bIsHitInvincible = false;
	OnInvincibilityChanged.Broadcast(false);
}
