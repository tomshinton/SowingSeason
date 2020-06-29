#include "Runtime/Build/Public/BuildingManager/BuildingManager.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Events/BuildEvents.h"

#include <Runtime/Buildings/Public/Building.h>
#include <Runtime/Buildings/Public/LinkableBuilding.h>

#include <ObjectMessaging/Public/Listener/ObjectMessagingListenerInterface.h>

DEFINE_LOG_CATEGORY_STATIC(BuildingManager, Log, Log)

namespace BuildingManagerPrivate
{
#if WITH_EDITOR
	const FName BuildingFolderPath = "Buildings";
#endif //WITH_EDITOR
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
	switch (InEv.BuildingData->BuildMode)
	{
	case EBuildMode::Single:
		TrySpawnSingleBuilding(InEv);
		break;
	case EBuildMode::Grid:
		break;
	case EBuildMode::Linear:
		TrySpawnLinearBuildings(InEv);
		break;
	}
}

void UBuildingManager::TrySpawnSingleBuilding(const FBuildCompleteEvent& InEv)
{
	if (ABuilding* SpawnedBuilding = World->SpawnActor<ABuilding>(InEv.BuildingData->BuildingClass.ResolveClass(), InEv.BuildingFoundation.Transform()))
	{
#if WITH_EDITOR
		SpawnedBuilding->SetFolderPath(BuildingManagerPrivate::BuildingFolderPath);
#endif //WITH_EDITOR
	}
}

void UBuildingManager::TrySpawnLinearBuildings(const FBuildCompleteEvent& InEv)
{
	for (const FFoundationPoint& Point : InEv.BuildingFoundation.Points)
	{
		if (ALinkableBuilding* SpawnedBuilding = World->SpawnActor<ALinkableBuilding>(InEv.BuildingData->BuildingClass.ResolveClass(), FTransform(FRotator::ZeroRotator, Point.Location, FVector(1.f))))
		{
#if WITH_EDITOR
			SpawnedBuilding->SetFolderPath(BuildingManagerPrivate::BuildingFolderPath);
#endif //WITH_EDITOR
		}
	}
}
