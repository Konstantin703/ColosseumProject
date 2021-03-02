// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColosseumProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"

AColosseumProjectGameMode::AColosseumProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/MainCharacter/MainCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
