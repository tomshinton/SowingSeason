#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "Runtime/AICore/Public/VillagerInterface.h"

#include "VillagerManager.generated.h"

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

	void BuildSpawnLocations(UNavigationSystemV1& InNavSys);
};