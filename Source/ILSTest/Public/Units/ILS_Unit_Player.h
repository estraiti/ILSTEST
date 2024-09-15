// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILS_Unit.h"
#include "ILS_Unit_Player.generated.h"
 
class UCameraComponent;
class UCharacterMovementComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class ILSTEST_API AILS_Unit_Player : public AILS_Unit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AILS_Unit_Player();

//Components
protected:

	//Camera setup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoom;

	UCharacterMovementComponent* MovementComponent;

protected:

	//Movement
	void MoveForward(float value);
	void MoveRight(float value);

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
