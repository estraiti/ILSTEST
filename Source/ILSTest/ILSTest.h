// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Punch			UMETA(DisplayName = "Punch"),
	// 4 Q
	Dash	     	UMETA(DisplayName = "Dash"),
	// 6 E
	Telekinesis		UMETA(DisplayName = "Telekinesis"),
	// 7 Crouch
	Crouch		    UMETA(DisplayName = "Crouch"),
	// 8 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump")
};