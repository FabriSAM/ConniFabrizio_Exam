// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConniFabrizio_ExamGameMode.h"
#include "ConniFabrizio_ExamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AConniFabrizio_ExamGameMode::AConniFabrizio_ExamGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
