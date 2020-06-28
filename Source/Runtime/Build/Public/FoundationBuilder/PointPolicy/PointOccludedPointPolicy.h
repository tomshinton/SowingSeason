#pragma once

#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/PointValidationPolicy.h"

#include "Runtime/Build/Public/BuildSettings.h"

#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

DEFINE_LOG_CATEGORY_STATIC(PointOcclusedPolicyLog, Log, Log)

namespace PointOccludedPolicyPrivate
{
	const FName TraceTag = TEXT("PointOccludedTrace");
}

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarDebugOcclusionRender(TEXT("PlacementPolicy.Occlusion.DrawDebugTrace"), 0, TEXT("Debug draw point occlusion traces"));
#endif //!UE_BUILD_SHIPPING

class FPointOccludedPointPolicy : public FPointValidationPolicy
{
public:

	FPointOccludedPointPolicy()
		: World(nullptr)
		, BuildSettings(nullptr)
		, WorldSettings(nullptr)
		, CollisionShape()
	{};

	FPointOccludedPointPolicy(UWorld& InWorld)
	: World(&InWorld)
	, BuildSettings(GetDefault<UBuildSettings>())
	, WorldSettings(GetDefault<UWorldGridSettings>())
	{
		CollisionShape = FCollisionShape::MakeBox(WorldSettings->GetCellBounds());
		QueryParams.TraceTag = PointOccludedPolicyPrivate::TraceTag;
	};

	FORCEINLINE bool Run(const FFoundationPoint& InPoint) override
	{
		TArray<FOverlapResult> OverlapRes;

		if (World != nullptr)
		{
			World->OverlapMultiByChannel(OverlapRes, InPoint.Location, FQuat::Identity, BuildSettings->PlacementOverlapChannel, CollisionShape, QueryParams);

#if !UE_BUILD_SHIPPING
			if (CVarDebugOcclusionRender.GetValueOnAnyThread())
			{
				World->DebugDrawTraceTag = PointOccludedPolicyPrivate::TraceTag;
			}
#endif //!UE_BUILD_SHIPPING
		}
		else
		{
			UE_LOG(PointOcclusedPolicyLog, Warning, TEXT("World is null in policy"));
		}

		return OverlapRes.Num() == 0;
	}

	EPolicyFailReason GetFailReason() const override
	{
		return EPolicyFailReason::Overlapping;
	}

private:

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	const UBuildSettings* BuildSettings;

	UPROPERTY()
	const UWorldGridSettings* WorldSettings;

	FCollisionShape CollisionShape;
	FCollisionQueryParams QueryParams;
};