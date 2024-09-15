// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ILS_Unit_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AILS_Unit_AIController::AILS_Unit_AIController()
{
	
}

void AILS_Unit_AIController::BeginPlay() 
{
	Super::BeginPlay();
	if (IsValid(BehaviorTree)) {
		RunBehaviorTree(BehaviorTree);
		Blackboard->SetValueAsObject("TargetActor", Cast<UObject>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)));
	}
}