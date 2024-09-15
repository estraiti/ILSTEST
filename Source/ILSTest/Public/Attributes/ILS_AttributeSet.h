// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ILS_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ILSTEST_API UILS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Health;
		ATTRIBUTE_ACCESSORS(UILS_AttributeSet, Health);

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData MaxHealth;
		ATTRIBUTE_ACCESSORS(UILS_AttributeSet, MaxHealth);

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Stamina;
		ATTRIBUTE_ACCESSORS(UILS_AttributeSet, Stamina);

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData MaxStamina;
		ATTRIBUTE_ACCESSORS(UILS_AttributeSet, MaxStamina);

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
