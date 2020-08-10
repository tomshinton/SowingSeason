#include "Runtime/AICore/Public/VillagerManager.h"

#include "Runtime/AICore/Public/Spawning/VillagerSpawnVolume.h"

#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/Engine/Public/EngineUtils.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#endif //!UE_BUILD_SHIPPING

DEFINE_LOG_CATEGORY_STATIC(VillagerManagerLog, Log, Log)

namespace VillagerManagerPrivate
{
	const uint8 NumSpawnPointsPerVolume = 10;
}

UVillagerManager::UVillagerManager()
	: World(nullptr)
	, PotentialSpawnLocations()
{

}

void UVillagerManager::Init(UWorld& InWorld)
{
	World = &InWorld;

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
	{
		NavSys->OnNavigationInitDone.AddLambda([WeakThis = TWeakObjectPtr<UVillagerManager>(this), NavSys]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->BuildSpawnLocations(*NavSys);
			}
		});
	}
}

void UVillagerManager::BuildSpawnLocations(UNavigationSystemV1& InNavSys)
{
	for (TActorIterator<AVillagerSpawnVolume> Itr(World); Itr; ++Itr)
	{
		if (AVillagerSpawnVolume* SpawnZone = *Itr)
		{
			if (!SpawnZone->IsTemplate())
			{
				if (UBoxComponent* BoxComp = SpawnZone->Volume)
				{
					for (uint8 i = 0; i < VillagerManagerPrivate::NumSpawnPointsPerVolume; ++i)
					{
						const FVector PotentialSpawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(BoxComp->GetComponentLocation(), BoxComp->GetScaledBoxExtent());
						FNavLocation NavData;

						if (InNavSys.ProjectPointToNavigation(PotentialSpawnLoc, NavData))
						{
							PotentialSpawnLocations.AddUnique(NavData.Location);

#if !UE_BUILD_SHIPPING
							DrawDebugSphere(World, NavData.Location, 20.f, 4, FColor::Red, false, 30.f);
#endif //!UE_BUILD_SHIPPING
						}
						else
						{
							UE_LOG(VillagerManagerLog, Warning, TEXT("%s is not navigable - not caching as a viable spawn point"), *PotentialSpawnLoc.ToString());
						}
					}
				}
			}
		}
	}

	UE_LOG(VillagerManagerLog, Log, TEXT("Caching %i villager spawn points"), PotentialSpawnLocations.Num());
}
