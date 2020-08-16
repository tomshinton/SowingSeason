// Sowing Season - Tom Shinton 2020

#include "Runtime/AICore/Public/VillagerManager.h"

#include "Runtime/AICore/Public/AISettings.h"
#include "Runtime/AICore/Public/Core/VillagerPawn.h"
#include "Runtime/AICore/Public/Spawning/VillagerSpawnVolume.h"

#include <Runtime/AIIdentity/Public/IdentityInterface.h>
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
	const float VillagerSpawnRate = 0.2f;

	const FName FamilyGeneratorName = TEXT("FamilyGenerator");

#if WITH_EDITOR
	const FName VillagerFolderPath = TEXT("Villagers");
#endif //WITH_EDITOR
}

UVillagerManager::UVillagerManager()
	: FamilyGenerator(CreateDefaultSubobject<UFamilyGeneratorComponent>(VillagerManagerPrivate::FamilyGeneratorName))
	, World(nullptr)
	, PotentialSpawnLocations()
	, AISettings(GetDefault<UAISettings>())
	, AsyncLoader(MakeUnique<FAsyncLoader>())
	, VillagerClass(nullptr)
	, Villagers()
	, NumVillagerRequests(0)
	, SpawnVillagerTimerHandle()
{
	FamilyGenerator->SetNewFamilyEmerganceChange(AISettings->NewFamilyEmerganceChance);
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

	if (AISettings != nullptr && !AISettings->VillagerClass.IsNull())
	{
		AsyncLoader->RequestLoad<UClass>(AISettings->VillagerClass, [WeakThis = TWeakObjectPtr<UVillagerManager>(this)](UClass& LoadedClass)
		{
			if (WeakThis.IsValid())
			{
				if (LoadedClass.IsChildOf(AVillagerPawn::StaticClass()))
				{
					WeakThis->VillagerClass = &LoadedClass;
				}
			}
		});
	}
}

void UVillagerManager::RequestVillagerSpawn()
{
	NumVillagerRequests++;

	FTimerManager& TimerManager = World->GetTimerManager();
	if (!TimerManager.IsTimerActive(SpawnVillagerTimerHandle))
	{
		TimerManager.SetTimer(SpawnVillagerTimerHandle, this, &UVillagerManager::SpawnVillager, VillagerManagerPrivate::VillagerSpawnRate, true, 1.f);
	}
}

void UVillagerManager::SpawnVillager()
{
	if (VillagerClass != nullptr)
	{
		if (AVillagerPawn* NewVillager = World->SpawnActor<AVillagerPawn>(VillagerClass, GetSpawnLocation(), FRotator::ZeroRotator))
		{
#if WITH_EDITOR
			NewVillager->SetFolderPath(VillagerManagerPrivate::VillagerFolderPath);
#endif //WITH_EDITOR

			Villagers.AddUnique(NewVillager);
			--NumVillagerRequests;

			AssignIdentity(*NewVillager);
		}
	}

	if (NumVillagerRequests <= 0)
	{
		FTimerDelegate SpawnShutdownDelegate;
		SpawnShutdownDelegate.BindLambda([WeakThis = TWeakObjectPtr<UVillagerManager>(this)]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->World->GetTimerManager().ClearTimer(WeakThis->SpawnVillagerTimerHandle);
			}
		});

		World->GetTimerManager().SetTimerForNextTick(SpawnShutdownDelegate);
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

FVector UVillagerManager::GetSpawnLocation() const
{
	return PotentialSpawnLocations[FMath::RandRange(0, PotentialSpawnLocations.Num() - 1)];
}

void UVillagerManager::AssignIdentity(AVillagerPawn& InVillager)
{
	if (IIdentityInterface* IdentityInterface = InVillager.GetInterface<IIdentityInterface>())
	{
		IdentityInterface->SetFamily(FamilyGenerator->GetFamilyForVillager(IdentityInterface->GetVillagerID()));
	}
}
