// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ILS_ExtraJump_GameplayAbility.h"
#include "ILS_Unit.h"

UILS_ExtraJump_GameplayAbility::UILS_ExtraJump_GameplayAbility()
{
	AbilityInputID = EGDAbilityInputID::None;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

//Grants +1 jump

void UILS_ExtraJump_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->JumpMaxCount = Character->JumpMaxCount + 1;
	Super::OnAvatarSet(ActorInfo, Spec);
}
