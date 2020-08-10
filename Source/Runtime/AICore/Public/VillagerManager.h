#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "Runtime/AICore/Public/VillagerInterface.h"
#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>

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

	//IVillagerInterface
	void RequestVillagerSpawn() override;
	//~IVillagerInterface

	void SpawnVillager();

	void BuildSpawnLocations(UNavigationSystemV1& InNavSys);
	FVector GetSpawnLocation() const;
	
	FTimerHandle SpawnVillagerTimerHandle;
};