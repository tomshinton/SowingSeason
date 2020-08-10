// Sowing Season - Tom Shinton 2020

#include "Runtime/AICore/Public/AIBlueprintLibrary.h"

#include "Runtime/AICore/Public/VillagerInterface.h"

#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

void UAIBlueprintLibrary::RequestVillagerSpawn(UObject* InWorldContext, const uint8 InNumToSpawn)
{
	if (InNumToSpawn > 0)
	{
		if (IVillagerInterface* VillagerInterface = TManagerPtr<IVillagerInterface>(*InWorldContext->GetWorld()).Get())
		{
			for (uint8 i = 0; i < InNumToSpawn; ++i)
			{
				VillagerInterface->RequestVillagerSpawn();
			}
		}
	}
}
