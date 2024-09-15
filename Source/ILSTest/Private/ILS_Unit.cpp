// Fill out your copyright notice in the Description page of Project Settings.


#include "ILS_Unit.h"
#include "AbilitySystemComponent.h"
#include "Abilities/ILS_GameplayAbility.h"
#include "Attributes/ILS_AttributeSet.h"

// Sets default values
AILS_Unit::AILS_Unit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GAS Setup
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AILS_Unit::GiveAbilities()
{
	//Abilities are granted
	
	for (TSubclassOf<UILS_GameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}
}

void AILS_Unit::GiveStartupEffects()
{
	//EfectsAreGranted

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	FPredictionKey Hitpredictionkey;
	EffectContext.AddSourceObject(this);
	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void AILS_Unit::PossessedBy(AController* NewController)
{
	//Initialize on controlled
	Super::PossessedBy(NewController);
	

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AttributeSet = AbilitySystemComponent->GetSet<UILS_AttributeSet>();

		GiveAbilities();
		GiveStartupEffects();
	}
}

void AILS_Unit::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	OnUnitLanded.Broadcast(this);
}



// Called when the game starts or when spawned
void AILS_Unit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AILS_Unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


