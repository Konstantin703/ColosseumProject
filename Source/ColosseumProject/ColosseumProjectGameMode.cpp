// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColosseumProjectGameMode.h"
#include "ColosseumProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AColosseumProjectGameMode::AColosseumProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
