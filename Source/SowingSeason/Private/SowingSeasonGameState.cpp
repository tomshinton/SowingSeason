// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Public/SowingSeasonGameState.h"

#include <Runtime/Build/Public/BuildingManager/BuildingManager.h>
#include <Runtime/Construction/Public/ConstructionManager.h>
#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionManager.h>

namespace SowingSeasonGameStatePrivate
{
	const uint8 ManagerCount = 3;
}

void ASowingSeasonGameState::BuildManagerList()
{
	Super::BuildManagerList();

	ManagerList.Reserve(SowingSeasonGameStatePrivate::ManagerCount);

	ManagerList.Emplace(UGridProjectionManager::StaticClass(), [this](UObject& InSpawnedManager) { InitialiseGridLocationManager(InSpawnedManager); });
	ManagerList.Emplace(UBuildingManager::StaticClass(), [this](UObject& InSpawnedManager) { InitialiseBuildingManager(InSpawnedManager); });
	ManagerList.Emplace(UConstructionManager::StaticClass(), [this](UObject& InSpawnedManager) { InitialiseConstructionManager(InSpawnedManager); });

#if !UE_BUILD_SHIPPING
	checkf(ManagerList.Num() == SowingSeasonGameStatePrivate::ManagerCount, TEXT("Manager count exceeds static manager count - make sure the correct amount are reserved prior to population"))
#endif //!UE_BUILD_SHIPPING
}

void ASowingSeasonGameState::InitialiseGridLocationManager(UObject& InManager)
{
	UGridProjectionManager* Manager = CastChecked<UGridProjectionManager>(&InManager);
	Manager->Init(*GetWorld());
}

void ASowingSeasonGameState::InitialiseBuildingManager(UObject& InManager)
{
	UBuildingManager* Manager = CastChecked<UBuildingManager>(&InManager);
	Manager->Init(*GetWorld());
}

void ASowingSeasonGameState::InitialiseConstructionManager(UObject& InManager)
{
	UConstructionManager* Manager = CastChecked<UConstructionManager>(&InManager);
	Manager->Init(*GetWorld());
}
