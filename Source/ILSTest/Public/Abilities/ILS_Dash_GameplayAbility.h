// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ILS_GameplayAbility.h"
#include "ILS_Dash_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ILSTEST_API UILS_Dash_GameplayAbility : public UILS_GameplayAbility
{
	GENERATED_BODY()
	
public:

	UILS_Dash_GameplayAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION()
	virtual void OnOwnerLanded(AILS_Unit* OwnerUnit);

protected:
	//Dash Variables
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DashStrength;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Duration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int MaxAerialDashes;

	UPROPERTY(BlueprintReadOnly)
	int AerialDashes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DashMontage; 

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
};
