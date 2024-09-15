// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/ILS_Unit_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/ILS_GameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AILS_Unit_Player::AILS_Unit_Player()
{
	//Create Components
		//Camera arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	CameraBoom->bUsePawnControlRotation = true;

	//Camera
	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(CameraBoom);

	//Setup movement
	bUseControllerRotationYaw = false;
	MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = false;
	
}

void AILS_Unit_Player::MoveForward(float value)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	AddMovementInput(ForwardVector * value);
}

void AILS_Unit_Player::MoveRight(float value)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	AddMovementInput(RightVector * value);
}

// Called to bind functionality to input
void AILS_Unit_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement inputs
	PlayerInputComponent->BindAxis("MoveForward", this, &AILS_Unit_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AILS_Unit_Player::MoveRight);

	//Camera inputs
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	//Gas Abilities
	if (AbilitySystemComponent && PlayerInputComponent)
	{
		//FTopLevelAssetPath(FName("/ILSTest/ILSTest"), FName("EGDAbilityInputID"))
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGDAbilityInputID", static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);
	}
}

FVector AILS_Unit_Player::GetPawnViewLocation() const
{
	if (IsValid(TPSCameraComponent)) {
		return TPSCameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}
