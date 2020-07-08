// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Cheats/SowingSeasonCheatManager.h"

#include <ObjectMessaging/Public/Sender/ObjectMessagingFunctions.h>
#include <Runtime/Build/Public/BuildInterface.h>
#include <Runtime/Construction/Public/Events/ConstructionEvents.h>

namespace ConstructionCheatsPrivate
{
	const float DebugConstructionRate = 1.f / 30.f; //about 30 times a second
}

USowingSeasonCheatManager::USowingSeasonCheatManager()
	: DebugConstructionHandle()
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

void USowingSeasonCheatManager::EnableDebugConstruction()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(DebugConstructionHandle))
	{
		FTimerDelegate UpdateConstructionDelegate;
		UpdateConstructionDelegate.BindLambda([WeakThis = TWeakObjectPtr<USowingSeasonCheatManager>(this), this]()
		{
			if (WeakThis.IsValid())
			{
				ObjectMessagingFunctions::SendMessage<FUpdateConstructionRequest>(*GetWorld()->GetGameInstance(), FUpdateConstructionRequest(ConstructionCheatsPrivate::DebugConstructionRate));
			}
		});

		GetWorld()->GetTimerManager().SetTimer(DebugConstructionHandle, UpdateConstructionDelegate, ConstructionCheatsPrivate::DebugConstructionRate, true);
	}
}

void USowingSeasonCheatManager::DisableDebugConstruction()
{
	GetWorld()->GetTimerManager().ClearTimer(DebugConstructionHandle);
}
