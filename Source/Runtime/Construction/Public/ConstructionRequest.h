#pragma once

#include <Runtime/Build/Public/Events/BuildEvents.h>

#include "Runtime/Construction/Public/ConstructionData/ConstructionData.h"

#include "ConstructionRequest.generated.h"

class AConstructionSite;

DECLARE_MULTICAST_DELEGATE(FOnRequestCompleted);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRequestAdvanced, const float);

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
		LoadedConstructionData->GetTimeForState(CurrentState);
	};

	~FConstructionRequest()
	{
		if (OnRequestCompleted.IsBound())
		{
			OnRequestCompleted.Broadcast();
		}
	}

	void Advance(const float InAmount);
	void Complete();

	UPROPERTY()
	FBuildCompleteEvent Source;

	UPROPERTY()
	const UConstructionData* LoadedConstructionData;

	UPROPERTY()
	AConstructionSite* SpawnedConstructionSite;

	FGuid ID;

	float Progress;
	float TargetProgress;
	EConstructionState CurrentState;

	FOnRequestCompleted OnRequestCompleted;
	FOnRequestAdvanced OnRequestAdvanced;
};