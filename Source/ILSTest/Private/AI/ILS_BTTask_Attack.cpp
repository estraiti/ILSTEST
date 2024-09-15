// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ILS_BTTask_Attack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"

EBTNodeResult::Type UILS_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	UAbilitySystemComponent* OwnerAbilityComp = controller->GetPawn()->FindComponentByClass<UAbilitySystemComponent>();
	OwnerAbilityComp->TryActivateAbilityByClass(AttacktoActivate);
	return EBTNodeResult::Type();
}
