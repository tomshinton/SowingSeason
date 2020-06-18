#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>

#include "BuildingManager.generated.h"

struct FBuildCompleteEvent;
class FFoundation;
class UBuildingData;

UCLASS(MinimalAPI)
class UBuildingManager : public UObject 
{
	GENERATED_BODY()

public:

	BUILD_API void Init(const UWorld& InWorld);

private:

	void BindEvents();

	void OnBuildingComplete(const FBuildCompleteEvent& InEv);

	void TrySpawnSingleBuilding(const UBuildingData& InData, const FFoundation& InFoundation);

	UPROPERTY()
	UWorld* World;
};