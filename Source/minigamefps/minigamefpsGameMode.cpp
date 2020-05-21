// Copyright Epic Games, Inc. All Rights Reserved.

#include "minigamefpsGameMode.h"
#include "minigamefpsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AminigamefpsGameMode::AminigamefpsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
