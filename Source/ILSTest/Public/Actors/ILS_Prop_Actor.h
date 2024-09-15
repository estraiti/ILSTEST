// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ILS_Telekinesis_Interface.h"
#include "GameplayEffect.h"
#include "ILS_Prop_Actor.generated.h"

UCLASS()
class ILSTEST_API AILS_Prop_Actor : public AActor, public IILS_Telekinesis_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AILS_Prop_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//AActor* TelekineticUser;

	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshcomponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Telekinesis")
	FVector TelekinesisOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Telekinesis")
	float TelekinesisHoldSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Telekinesis")
	float TelekimesisReleaseSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Telekinesis|GAS")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	AActor* TelekineticUser;

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Telekinesis_Initial_Interaction(AActor* TelekinesisUser) override;
	virtual void Telekinesis_Release_Interaction(AActor* TelekinesisUser) override;
};
