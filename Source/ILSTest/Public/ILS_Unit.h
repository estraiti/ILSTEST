// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ILS_Unit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitLanded, AILS_Unit*, UNIT);

class UAbilitySystemComponent;

UCLASS()
class ILSTEST_API AILS_Unit : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AILS_Unit();

	//delegate for when the unit lands
	UPROPERTY()
	FOnUnitLanded OnUnitLanded;

protected:

	//GAS setup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPriviteAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	//AtributeSet
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPriviteAccess = "true"))
	const class UILS_AttributeSet* AttributeSet;

	// Default abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPriviteAccess = "true"))
	TArray<TSubclassOf<class UILS_GameplayAbility>> CharacterAbilities;

	// Default effects 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void GiveAbilities();

	virtual void GiveStartupEffects();

	//Init
	virtual void PossessedBy(AController* NewController) override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
