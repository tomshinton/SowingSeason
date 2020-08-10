// Sowing Season - Tom Shinton 2020

#include "Runtime/Construction/Public/ConstructionRequest.h"

#if !UE_BUILD_SHIPPING
#include <Runtime/Build/Public/BuildingData/BuildingData.h>
#endif //!UE_BUILD_SHIPPING

DEFINE_LOG_CATEGORY_STATIC(ConstructionRequestLog, Log, Log)

void FConstructionRequest::Advance(const float InAmount)
{
	if (CurrentState != EConstructionState::Finished)
	{
		Progress = FMath::Clamp(Progress + InAmount, 0.f, TargetProgress);

		if (Progress >= TargetProgress)
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(ConstructionRequestLog, Log, TEXT("Advancing construction state of %s"), *Source.BuildingData->NameReadable);
#endif //!UE_BUILD_SHIPPING

			const uint8 CurrStateAsInt = static_cast<uint8>(CurrentState);
			CurrentState = static_cast<EConstructionState>(CurrStateAsInt + 1);

			if (CurrentState != EConstructionState::Finished)
			{
				TargetProgress = LoadedConstructionData->GetTimeForState(CurrentState);
			}
		}

		OnRequestAdvanced.Broadcast(FMath::Clamp(Progress / TargetProgress, 0.f, 1.f), CurrentState);
	}
}

void FConstructionRequest::Complete()
{
	OnRequestCompleted.Broadcast();
}
