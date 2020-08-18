// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "BuildingManager.generated.h"

struct FConstructionCompleteEvent;
struct FBuildCompleteEvent;

class ABuilding;
class FFoundation;
class UBuildingData;

UCLASS(MinimalAPI)
class UBuildingManager : public UObject 
{
	GENERATED_BODY()

public:

	UBuildingManager();

	BUILD_API void Init(UWorld& InWorld);

private:

	void BindEvents();

	void OnBuildingComplete(const FBuildCompleteEvent& InEv);

	ABuilding* TrySpawnBuilding(const FBuildCompleteEvent& InEv);

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	TArray<ABuilding*> Manifest;
};
