// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ILS_AttackCombo_GameplayAbility.h"
#include "ILS_Unit.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "AbilitySystemComponent.h"

UILS_AttackCombo_GameplayAbility::UILS_AttackCombo_GameplayAbility()
{
	AbilityInputID = EGDAbilityInputID::Punch;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Punch")));
	Combometer = 0;
	CanAttack = true;
}

//Dash
void UILS_AttackCombo_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}	
	
		if (IsValid(OwnerAnimInstance) && IsValid(MeleeMontages[Combometer]))
		{
			//Play Next Combo Montage
			UAnimMontage* TargetMontage = MeleeMontages[Combometer];
			UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargetMontage,1.f,NAME_None,true,1.f,0.f,false);
			MontageTask->OnBlendOut.AddDynamic(this, &UILS_AttackCombo_GameplayAbility::OnMontageCompleted);
			MontageTask->OnCompleted.AddDynamic(this, &UILS_AttackCombo_GameplayAbility::OnMontageCompleted);
			MontageTask->ReadyForActivation();
			ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
			//IF the hit box is null create hit box
			if (Hitbox == nullptr) 
			{
				Hitbox = NewObject<UBoxComponent>(Character, UBoxComponent::StaticClass(),TEXT("Hitbox"));
				Hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
				Hitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
				Hitbox->SetHiddenInGame(false, false);
				Hitbox->SetVisibility(true);
				Hitbox->SetBoxExtent(FVector(25.f, 25.f, 25.f), false);
				Hitbox->SetGenerateOverlapEvents(true);
				Hitbox->IgnoreActorWhenMoving(Character, true);
				Hitbox->OnComponentBeginOverlap.AddDynamic(this, &UILS_AttackCombo_GameplayAbility::OnHitboxOverlap);
			}
			if (Hitboxsockets.IsValidIndex(Combometer)) 
			{
				Hitbox->AttachToComponent(Character->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, Hitboxsockets[Combometer]);
				Hitbox->RegisterComponent();
			}
			CanAttack = false;
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
}

bool UILS_AttackCombo_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	return CanAttack;
}


void UILS_AttackCombo_GameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
}

void UILS_AttackCombo_GameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (CanActivateAbility(Handle, ActorInfo)) 
	{
		ActivateAbility(Handle,ActorInfo, ActivationInfo,nullptr);
	}
}

void UILS_AttackCombo_GameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UILS_AttackCombo_GameplayAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	CanAttack = true;
	Combometer = 0;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

}

void UILS_AttackCombo_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(Character->GetMesh())) 
	{
		OwnerAnimInstance = Character->GetMesh()->GetAnimInstance();
	}
}

void UILS_AttackCombo_GameplayAbility::OnMontageCompleted()
{
	
	if (Hitbox != nullptr)
	{
		Hitbox->DestroyComponent();
		Hitbox = nullptr;
	}
	CanAttack = true;
	Combometer = 0;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UILS_AttackCombo_GameplayAbility::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* Othercomp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAbilitySystemComponent* TargetComponent = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	if (IsValid(TargetComponent)) {
		UGameplayEffect* DamageGameplayEffectHandle = NewObject<UGameplayEffect>(this, DamageGameplayEffect, TEXT("Damage"));
		FGameplayEffectContextHandle HitContextHandle;
		FPredictionKey Hitpredictionkey;
		TargetComponent->ApplyGameplayEffectToSelf(DamageGameplayEffectHandle,1,HitContextHandle,Hitpredictionkey);
		//Launch
		if (MeleeMontages[Combometer] == MeleeMontages.Last()) {
			ACharacter* Character = CastChecked<ACharacter>(OtherActor);
			FVector FinalLaunchVelocity = Hitbox->GetOwner()->GetActorRotation().RotateVector(LaunchVelocity);
			Character->LaunchCharacter(FinalLaunchVelocity,false, false);
		}
		//Check combo conditions
		if (!CanAttack)
		{
			//Update Combometer
			Combometer++;
			if (!MeleeMontages.IsValidIndex(Combometer))
			{
				Combometer = 0; //ResetcomboMetter
			}
			else
			{
				CanAttack = true;//Can execute next combo
				bRetriggerInstancedAbility = true;
			}
		}
	}
}
