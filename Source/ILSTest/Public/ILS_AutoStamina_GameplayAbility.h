// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ILS_GameplayAbility.h"
#include "ILS_AutoStamina_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ILSTEST_API UILS_AutoStamina_GameplayAbility : public UILS_GameplayAbility
{
	GENERATED_BODY()
	
public:

	UILS_AutoStamina_GameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> TargetGameplayEffect;
};
