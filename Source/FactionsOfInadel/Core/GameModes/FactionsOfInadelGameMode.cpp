// Copyright Epic Games, Inc. All Rights Reserved.

#include "FactionsOfInadelGameMode.h"
#include "FactionsOfInadelPlayerController.h"
#include "../MainCharacter/FactionsOfInadelCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFactionsOfInadelGameMode::AFactionsOfInadelGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFactionsOfInadelPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/FoI/Core/Characters/MainCharacter/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}