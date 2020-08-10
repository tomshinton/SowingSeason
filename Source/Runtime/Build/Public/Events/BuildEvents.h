// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Build/Public/Foundation/Foundation.h"

#include "BuildEvents.generated.h"

class UBuildingData;

USTRUCT()
struct FBuildCompleteEvent
{
	GENERATED_BODY()

public:

	FBuildCompleteEvent() {};

	FBuildCompleteEvent(const UBuildingData& InBuildingData, const FFoundation& InBuildingFoundation)
		: BuildingData(&InBuildingData)
		, BuildingFoundation(InBuildingFoundation)
	{};

	UPROPERTY()
	const UBuildingData* BuildingData;

	FFoundation BuildingFoundation;
};

USTRUCT()
struct FConstructionCompleteEvent
{
	GENERATED_BODY()

public:

	FConstructionCompleteEvent() {};

	FConstructionCompleteEvent(const FBuildCompleteEvent& InSourceBuildEvent)
		: SourceBuildEvent(InSourceBuildEvent)
	{};

	FBuildCompleteEvent SourceBuildEvent;
};
