// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ILS_Dash_GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ILS_Unit.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

UILS_Dash_GameplayAbility::UILS_Dash_GameplayAbility()
{
	AbilityInputID = EGDAbilityInputID::Dash;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dash")));

	//Dash Variables
	DashStrength = 4000.f;
	Duration = 0.2f;
	AerialDashes = MaxAerialDashes;
}

//Dash
void UILS_Dash_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		UCharacterMovementComponent* Movementcomponent = Character->GetCharacterMovement();
		if (Movementcomponent->MovementMode.GetValue() == EMovementMode::MOVE_Falling) {
			AerialDashes--;
		}
		FVector DashDirection = Character->GetActorForwardVector();
		FVector ExitVelocity = Character->GetVelocity();
		UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, "None", DashDirection, DashStrength, Duration, false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, ExitVelocity, 0.f, false);
		DashTask->ReadyForActivation();
		Character->PlayAnimMontage(DashMontage);
	}
}

bool UILS_Dash_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (Character->GetCharacterMovement()->MovementMode.GetValue() == EMovementMode::MOVE_Falling && AerialDashes <= 0) {
		return false;
	}
	return true;
}

//Dash Hold Stopped
void UILS_Dash_GameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UILS_Dash_GameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UILS_Dash_GameplayAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

}

void UILS_Dash_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	AILS_Unit* Character = CastChecked<AILS_Unit>(ActorInfo->AvatarActor.Get());
	Character->OnUnitLanded.AddDynamic(this, &UILS_Dash_GameplayAbility::OnOwnerLanded);
	AerialDashes = MaxAerialDashes;
}

//Owner landed reset aerial dashes
void UILS_Dash_GameplayAbility::OnOwnerLanded(AILS_Unit* OwnerUnit)
{
	AerialDashes = MaxAerialDashes;
}

void UILS_Dash_GameplayAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
