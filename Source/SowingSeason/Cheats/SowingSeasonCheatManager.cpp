// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Cheats/SowingSeasonCheatManager.h"

#include <ObjectMessaging/Public/Sender/ObjectMessagingFunctions.h>
#include <Runtime/Build/Public/BuildInterface.h>
#include <Runtime/Construction/Public/Events/ConstructionEvents.h>

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

void USowingSeasonCheatManager::ProgressAllConstructionRequests(const float InAmount)
{
	ObjectMessagingFunctions::SendMessage<FUpdateConstructionRequest>(*GetWorld()->GetGameInstance(), FUpdateConstructionRequest(InAmount));
}
