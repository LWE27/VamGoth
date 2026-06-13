// Copyright Epic Games, Inc. All Rights Reserved.

#include "VamGothCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "VamGoth.h"
#include "Kismet/GameplayStatics.h"

AVamGothCharacter::AVamGothCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, 500.f, 0.f);
	Movement->JumpZVelocity = 500.f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.f;
	Movement->MinAnalogWalkSpeed = 20.f;
	Movement->BrakingDecelerationWalking = 2000.f;
	Movement->BrakingDecelerationFalling = 1500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AVamGothCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogVamGoth, Error, TEXT("%s has no Enhanced Input Component."), *GetNameSafe(this));
		return;
	}

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVamGothCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVamGothCharacter::Look);
	EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AVamGothCharacter::Look);

	EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Started, this, &AVamGothCharacter::DoAttack);
}

void AVamGothCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AVamGothCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	DoLook(LookVector.X, LookVector.Y);
}

void AVamGothCharacter::DoMove(float Right, float Forward)
{
	AController* OwningController = GetController();

	if (!OwningController)
	{
		return;
	}

	const FRotator ControlRotation = OwningController->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void AVamGothCharacter::DoLook(float Yaw, float Pitch)
{
	if (!GetController())
	{
		return;
	}

	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void AVamGothCharacter::DoAttack()
{
	if (!CanAttack)
	{
		return;
	}

	CanAttack = false;
	IsAttacking = true;
	SaveAttack = true;

	UE_LOG(LogTemp, Display, TEXT("Do Attack, Attack Count: %d"), AttackCount);

	UAnimMontage* MontageToPlay = nullptr;

	switch (AttackCount)
	{
	case 0:
		MontageToPlay = AttackComboOne;
		AttackCount = 1;
		break;

	case 1:
		MontageToPlay = AttackComboTwo;
		AttackCount = 2;
		break;

	case 2:
		MontageToPlay = AttackComboThree;
		ResetCombo();
		break;

	default:
		ResetCombo();
		break;
	}

	if (MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
		PlayComboSound();
	}

	GetWorldTimerManager().SetTimer(
		AttackIntervalTimerHandle,
		this,
		&AVamGothCharacter::ResetAttackInterval,
		AttackInterval,
		false
	);
}

void AVamGothCharacter::ResetCombo()
{
	IsAttacking = false;
	SaveAttack = false;
	AttackCount = 0;
}

void AVamGothCharacter::ResetAttackInterval()
{
	CanAttack = true;
	UE_LOG(LogTemp, Display, TEXT("Attack reset"));
}

void AVamGothCharacter::PlayComboSound()
{
	switch (AttackCount)
	{
	case 0:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ComboOneSound, this->GetActorLocation());
		break;
	case 1:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ComboTwoSound, this->GetActorLocation());
		break;
	case 2:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ComboThreeSound, this->GetActorLocation());
		break;
	default:
		return;
	}
}
