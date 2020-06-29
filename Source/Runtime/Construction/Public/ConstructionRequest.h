#pragma once

#include <Runtime/Build/Public/Events/BuildEvents.h>

#include "Runtime/Construction/Public/ConstructionData/ConstructionData.h"

#include "ConstructionRequest.generated.h"

USTRUCT()
struct FConstructionRequest
{
	GENERATED_BODY()

public:

	FConstructionRequest()
		: Source(FBuildCompleteEvent())
		, LoadedConstructionData(nullptr)
		, ID(FGuid())
		, Progress(0.f)
		, TargetProgress(0.f)
	{};

	FConstructionRequest(const FBuildCompleteEvent& InSource, const UConstructionData& InConstructionData)
		: Source(InSource)
		, LoadedConstructionData(&InConstructionData)
		, ID(FGuid::NewGuid())
		, Progress(0.f)
		, TargetProgress(0.f)
	{
		CurrentState = static_cast<EConstructionState>(0);
	};

	void operator+=(const float InAmount);;

	UPROPERTY()
	FBuildCompleteEvent Source;

	UPROPERTY()
	const UConstructionData* LoadedConstructionData;

	FGuid ID;

	float Progress;
	float TargetProgress;
	EConstructionState CurrentState;
};