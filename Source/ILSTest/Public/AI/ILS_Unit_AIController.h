// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree\BehaviorTree.h"
#include "ILS_Unit_AIController.generated.h"

/**
 * 
 */
UCLASS()
class ILSTEST_API AILS_Unit_AIController : public AAIController
{
	GENERATED_BODY()
public:

	AILS_Unit_AIController();

	virtual void BeginPlay();

protected:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

};
