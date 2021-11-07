// Copyright Epic Games, Inc. All Rights Reserved.

#include "CabinGameMode.h"
#include "CabinCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACabinGameMode::ACabinGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
