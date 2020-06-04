// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Cheats/SowingSeasonCheatManager.h"

#include <Runtime/Build/Public/BuildInterface.h>

USowingSeasonCheatManager::USowingSeasonCheatManager()
{

}

void USowingSeasonCheatManager::StartBuildingFromClassPath(const FSoftObjectPath& InClassToBuild)
{
	if (IBuildInterface* BuildInterface = GetLocalPawn()->GetInterface<IBuildInterface>())
	{
		BuildInterface->StartBuildFromClass(InClassToBuild);
	}
}
