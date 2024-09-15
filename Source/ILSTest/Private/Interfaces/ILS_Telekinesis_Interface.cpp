// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ILS_Telekinesis_Interface.h"

// Add default functionality here for any IILS_Telekinesis_Interface functions that are not pure virtual.

bool IILS_Telekinesis_Interface::Telekinesis_Initial_Interaction(AActor* TelekinesisUser) //True if the actor accepts the telekinesis control
{
	return true;
}

void IILS_Telekinesis_Interface::Telekinesis_Release_Interaction(AActor* TelekinesisUser)
{
}