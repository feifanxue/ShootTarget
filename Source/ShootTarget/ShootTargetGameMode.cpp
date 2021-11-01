// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootTargetGameMode.h"
#include "ShootTargetHUD.h"
#include "ShootTargetCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootTargetGameMode::AShootTargetGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShootTargetHUD::StaticClass();
}
