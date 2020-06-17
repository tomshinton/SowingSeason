#pragma once

#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

#include "BuildEvents.generated.h"

class UBuildingData;

USTRUCT()
struct FBuildCompleteEvent
{
	GENERATED_BODY()

public:

	FBuildCompleteEvent() {};

	FBuildCompleteEvent(const UBuildingData& InBuildingData, const TArray<FFoundationPoint>& InGeneratedPoints)
		: BuildingData(&InBuildingData)
		, GeneratedPoints(InGeneratedPoints)
	{};

	UPROPERTY()
	const UBuildingData* BuildingData;

	TArray<FFoundationPoint> GeneratedPoints;
};