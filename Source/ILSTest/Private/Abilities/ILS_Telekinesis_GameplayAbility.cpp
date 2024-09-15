// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ILS_Telekinesis_GameplayAbility.h"
#include "GameFramework/Character.h"
#include "interfaces/ILS_Telekinesis_Interface.h"

UILS_Telekinesis_GameplayAbility::UILS_Telekinesis_GameplayAbility()
{
	AbilityInputID = EGDAbilityInputID::Telekinesis;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Telekinesis")));
}

void UILS_Telekinesis_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		FVector LineTraceStart;
		FRotator LinetraceRotation;
		Character->GetActorEyesViewPoint(LineTraceStart,LinetraceRotation);
		FVector LineTraceEnd = LineTraceStart + ( LinetraceRotation.Vector() * TelekinesisRange);
		FHitResult  Outhit;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Character);
		bool hit = Character->GetWorld()->LineTraceSingleByChannel(Outhit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
		DrawDebugLine(Character->GetWorld(), LineTraceStart, LineTraceEnd, FColor::Red, false, 60.0f, 0.f, 1.f);
		if (hit) {
			AActor* TargetActor = Outhit.GetActor();
			IILS_Telekinesis_Interface* TelekinesisInterface = Cast<IILS_Telekinesis_Interface>(TargetActor);	
			if (TelekinesisInterface)//Does actor have Telekinesis target interface
			{ 
				if (TelekinesisInterface->Telekinesis_Initial_Interaction(Character))
				{
					//If the target is fully valid, commit Ability
					if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
					{
						EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
					}
					TelekinesisTarget = TargetActor;
				}
				else 
				{
					EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				}
			}
			else
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			}
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		
	}
}

bool UILS_Telekinesis_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	return true;
}

void UILS_Telekinesis_GameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UILS_Telekinesis_GameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (IsValid(TelekinesisTarget)) {
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		IILS_Telekinesis_Interface* TelekinesisInterface = Cast<IILS_Telekinesis_Interface>(TelekinesisTarget);
		TelekinesisInterface->Telekinesis_Release_Interaction(Character);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UILS_Telekinesis_GameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (IsValid(TelekinesisTarget)) {
		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		IILS_Telekinesis_Interface* TelekinesisInterface = Cast<IILS_Telekinesis_Interface>(TelekinesisTarget);
		TelekinesisInterface->Telekinesis_Release_Interaction(Character);
	}
}
