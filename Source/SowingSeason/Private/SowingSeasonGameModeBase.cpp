// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Public/SowingSeasonGameModeBase.h"

#include "SowingSeason/Public/SowingSeasonGameState.h"
#include "SowingSeason/Public/SowingSeasonPlayerController.h"

ASowingSeasonGameModeBase::ASowingSeasonGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = ASowingSeasonPlayerController::StaticClass();
	GameStateClass = ASowingSeasonGameState::StaticClass();
}
