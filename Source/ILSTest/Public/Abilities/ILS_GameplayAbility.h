// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ILSTest/ILSTest.h"
#include "ILS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ILSTEST_API UILS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UILS_GameplayAbility();

	//Abilities will activate if their input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGDAbilityInputID AbilityInputID = EGDAbilityInputID::None;

};
