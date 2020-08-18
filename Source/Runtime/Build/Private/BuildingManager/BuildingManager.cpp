// Sowing Season - Tom Shinton 2020

#include "Runtime/Build/Public/BuildingManager/BuildingManager.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Events/BuildEvents.h"

#include <Runtime/Buildings/Public/Building.h>

#include <ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h>

DEFINE_LOG_CATEGORY_STATIC(BuildingManager, Log, Log)

namespace BuildingManagerPrivate
{
#if WITH_EDITOR
	const FName BuildingFolderPath = "Buildings";
#endif //WITH_EDITOR
}

UBuildingManager::UBuildingManager()
	: World(nullptr)
	, Manifest()
{

}

void UBuildingManager::Init(UWorld& InWorld)
{
	World = &InWorld;

	BindEvents();
}

void UBuildingManager::BindEvents()
{
	if (IObjectMessagingListenerInterface* MessagingListener = Cast<IObjectMessagingListenerInterface>(GetWorld()->GetGameInstance()))
	{
		TWeakObjectPtr<UBuildingManager> WeakThis = TWeakObjectPtr<UBuildingManager>(this);
		MessagingListener->GetListener().Bind<FConstructionCompleteEvent>([WeakThis](const FConstructionCompleteEvent& InEv)
		{
			if (WeakThis.IsValid() && InEv.SourceBuildEvent.BuildingFoundation.IsValidFoundation())
			{
				WeakThis->OnBuildingComplete(InEv.SourceBuildEvent);
			}
		});
	}
}

void UBuildingManager::OnBuildingComplete(const FBuildCompleteEvent& InEv)
{
	if (ABuilding* SpawnedBuilding = TrySpawnBuilding(InEv))
	{
		Manifest.Add(SpawnedBuilding);
	}
}

ABuilding* UBuildingManager::TrySpawnBuilding(const FBuildCompleteEvent& InEv)
{
	if (ABuilding* SpawnedBuilding = World->SpawnActor<ABuilding>(InEv.BuildingData->BuildingClass.ResolveClass(), InEv.BuildingFoundation.Transform()))
	{
	#if WITH_EDITOR
		SpawnedBuilding->SetFolderPath(BuildingManagerPrivate::BuildingFolderPath);
#endif //WITH_EDITOR

		return SpawnedBuilding;
	}

	return nullptr;
}
