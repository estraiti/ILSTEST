// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ILS_Telekinesis_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UILS_Telekinesis_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ILSTEST_API IILS_Telekinesis_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool Telekinesis_Initial_Interaction(AActor* TelekinesisUser); //First Telekinesis interaction
	virtual void Telekinesis_Release_Interaction(AActor* TelekinesisUser); //Release telekinesis interaction
};
