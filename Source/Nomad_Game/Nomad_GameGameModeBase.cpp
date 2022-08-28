// Copyright Epic Games, Inc. All Rights Reserved.


#include "Nomad_GameGameModeBase.h"
#include "Player/GamePC.h"
#include "Player/StrategyCameraPawn.h"

ANomad_GameGameModeBase::ANomad_GameGameModeBase()
{
	PlayerControllerClass = AGamePC::StaticClass();
	DefaultPawnClass = AStrategyCameraPawn::StaticClass();
}
