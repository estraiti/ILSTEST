// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ILS_Prop_Actor.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
// Sets default values
AILS_Prop_Actor::AILS_Prop_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
	StaticMeshcomponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshcomponent;
	StaticMeshcomponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AILS_Prop_Actor::BeginPlay()
{
	Super::BeginPlay();
}

void AILS_Prop_Actor::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "HIT");
	if (IsValid(TelekineticUser) && TelekineticUser != OtherActor) 
	{
		UAbilitySystemComponent* TargetComponent = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
		if (IsValid(TargetComponent)) {
			UGameplayEffect* DamageGameplayEffectHandle = NewObject<UGameplayEffect>(this, DamageGameplayEffect, TEXT("Damage"));
			FGameplayEffectContextHandle HitContextHandle;
			FPredictionKey Hitpredictionkey;
			TargetComponent->ApplyGameplayEffectToSelf(DamageGameplayEffectHandle, 1, HitContextHandle, Hitpredictionkey);
		}
	}
}

// Called every frame
void AILS_Prop_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Move towards User
	if (IsValid(TelekineticUser))
	{
		FVector TargetPosition = TelekineticUser->GetActorLocation() + TelekineticUser->GetActorRotation().RotateVector(TelekinesisOffset);
		if ((GetActorLocation() - TargetPosition).Length() > (TelekinesisHoldSpeed * DeltaTime)) //Move towards the target point
		{
			FVector MovementDirection = TargetPosition - GetActorLocation();
			MovementDirection.Normalize();
			TargetPosition = GetActorLocation() + (MovementDirection * (TelekinesisHoldSpeed * DeltaTime));
		}
		SetActorLocation(TargetPosition);
	}
}

bool AILS_Prop_Actor::Telekinesis_Initial_Interaction(AActor* TelekinesisUser)
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	TelekineticUser = TelekinesisUser;
	StaticMeshcomponent->SetSimulatePhysics(false);
	StaticMeshcomponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	return true;
}

void AILS_Prop_Actor::Telekinesis_Release_Interaction(AActor* TelekinesisUser)
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	ACharacter* Character = CastChecked<ACharacter>(TelekinesisUser);
	FRotator ThrowRotation = Character->GetControlRotation();
	FVector FinalImpulse = ThrowRotation.Vector() * TelekimesisReleaseSpeed;
	StaticMeshcomponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshcomponent->SetSimulatePhysics(true);
	StaticMeshcomponent->SetNotifyRigidBodyCollision(true);
	StaticMeshcomponent->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	StaticMeshcomponent->OnComponentHit.AddDynamic(this, &AILS_Prop_Actor::OnActorHit);
	StaticMeshcomponent->AddImpulse(FinalImpulse,TEXT("NAME_None"), true);
}

