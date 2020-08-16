// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "Runtime/AICore/Public/VillagerInterface.h"
#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>

#include <Runtime/AIIdentity/Public/Family/FamilyGeneratorComponent.h>

#include "VillagerManager.generated.h"

class AVillagerPawn;
class UAISettings;
class UNavigationSystemV1;

UCLASS(MinimalAPI)
class UVillagerManager : public UObject
	, public IVillagerInterface
{
	GENERATED_BODY()
public:

	UVillagerManager();

	AICORE_API void Init(UWorld& InWorld);

private:

	//IVillagerInterface
	void RequestVillagerSpawn() override;
	//~IVillagerInterface

	void SpawnVillager();

	void BuildSpawnLocations(UNavigationSystemV1& InNavSys);
	FVector GetSpawnLocation() const;
	void AssignIdentity(AVillagerPawn& InVillager);

	UPROPERTY()
	UFamilyGeneratorComponent* FamilyGenerator;

	UPROPERTY()
	UWorld* World;

	TArray<FVector> PotentialSpawnLocations;

	UPROPERTY()
	const UAISettings* AISettings;

	TUniquePtr<FAsyncLoader> AsyncLoader;

	UPROPERTY()
	UClass* VillagerClass;

	UPROPERTY()
	TArray<AVillagerPawn*> Villagers;

	int32 NumVillagerRequests;

	FTimerHandle SpawnVillagerTimerHandle;
};
