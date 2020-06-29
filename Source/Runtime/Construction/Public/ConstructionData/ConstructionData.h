#pragma once

#include <Runtime/Engine/Classes/Engine/DataAsset.h>

#include "Runtime/Construction/Public/ConstructionState.h"

#include "ConstructionData.generated.h"

DEFINE_LOG_CATEGORY_STATIC(ConstructionDataLog, Log, Log)

UCLASS()
class UConstructionData : public UDataAsset
{
	GENERATED_BODY()

public:

	UConstructionData()
	{};

	const float GetTimeForState(const EConstructionState& InState) const
	{
		const int8 StateIndex = ConstructionStates.IndexOfByPredicate([&InState](const FConstructionStateMap& State)
		{
			return State.State == InState;
		});

		if (StateIndex != INDEX_NONE)
		{
			return ConstructionStates[StateIndex].Time;
		}
		else
		{
			UE_LOG(ConstructionDataLog, Error, TEXT("Could not find StateTime! Returning 0"));
			return 0.f;
		}
	}

	/** How does this building generate it's foundation? */
	UPROPERTY(EditDefaultsOnly, Category = "States")
	TArray<FConstructionStateMap> ConstructionStates;
};