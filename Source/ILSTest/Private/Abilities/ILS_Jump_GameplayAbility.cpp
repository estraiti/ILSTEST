// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ILS_Jump_GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ILS_Unit.h"
#include "DrawDebugHelpers.h"

UILS_Jump_GameplayAbility::UILS_Jump_GameplayAbility()
{
	AbilityInputID = EGDAbilityInputID::Jump;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

//Jump
void UILS_Jump_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
			//Wall jump
			FVector LineTraceStart = Character->GetActorLocation();
			FVector LineTraceEnd = LineTraceStart + (Character->GetActorForwardVector() * WalljumpCheckRange);
			FHitResult  Outhit;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(Character);
			bool hit = Character->GetWorld()->LineTraceSingleByChannel(Outhit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_WorldStatic, CollisionParams);
			if (hit) {
				FRotator NewCharacterRotation = Outhit.Normal.ToOrientationRotator();
				NewCharacterRotation = FRotator(0,NewCharacterRotation.Yaw,0);
				Character->SetActorRotation(NewCharacterRotation);
				FVector FinalLaunchVelocity = NewCharacterRotation.RotateVector(WalljumpLaunchVelocity);
				Character->LaunchCharacter(FinalLaunchVelocity,true,true);
				Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); //Jump Count Reset
			}
			else {
				//Air Jump
				Character->Jump();
			}
			//Debug
			if (DrawLinetrace) {
				DrawDebugLine(Character->GetWorld(), LineTraceStart, LineTraceEnd, FColor::Red, false, 60.0f, 0.f, 1.f);
			}
		}
		else {
			Character->Jump();
		}
	}
}

bool UILS_Jump_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	return IsValid(Character);
}

//Jump Hold Stopped
void UILS_Jump_GameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UILS_Jump_GameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UILS_Jump_GameplayAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();

}
