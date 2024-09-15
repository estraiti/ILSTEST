// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ILS_GameplayAbility.h"
#include "ILS_ExtraJump_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ILSTEST_API UILS_ExtraJump_GameplayAbility : public UILS_GameplayAbility
{
	GENERATED_BODY()

public:

	UILS_ExtraJump_GameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
