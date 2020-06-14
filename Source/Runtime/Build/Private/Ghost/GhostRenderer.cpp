#include "Runtime/Build/Public/Ghost/GhostRenderer.h"

#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
static TAutoConsoleVariable<int32> CVarDebugGhostRender(TEXT("GhostRenderer.DrawDebugGhost"), 0, TEXT("Draw GhostRenderer debug data"));
#endif //!UE_BUILD_SHIPPING

AGhostRenderer::AGhostRenderer(const FObjectInitializer& ObjectInitializer)
	: SourceBuildingData(nullptr)
	, LastRenderedPoints()
	, GhostRoot(FVector::ZeroVector)
	, GridSettings(GetDefault<UWorldGridSettings>())
	, GridProjection(*this)
	, CachedWorld(nullptr)
{

}

void AGhostRenderer::BeginPlay()
{
	Super::BeginPlay();
	
	CachedWorld = GetWorld();
	check(CachedWorld != nullptr);

	if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
	{
		GridProjectionInterface->GetOnRoundedPositionChanged().AddLambda([WeakThis = TWeakObjectPtr<AGhostRenderer>(this), this](const FVector& InNewLocation, const FVector& InOldLocation)
		{
			if (WeakThis.IsValid())
			{
				GhostRoot = InNewLocation;
			}
		});
	}
}

void AGhostRenderer::SetGhostInfo(const UBuildingData& InSourceBuildingData)
{
	SourceBuildingData = &InSourceBuildingData;
}

void AGhostRenderer::UpdateRender(const TArray<FFoundationPoint>& InPoints)
{
	LastRenderedPoints = InPoints;

#if !UE_BUILD_SHIPPING
	if (CVarDebugGhostRender.GetValueOnAnyThread())
	{
		FlushPersistentDebugLines(CachedWorld);
		for (const FFoundationPoint& Point : InPoints)
		{
			DrawDebugSphere(CachedWorld, Point.Location, GridSettings->GridCellSize * .5, 4.f, FColor::Green, true, -1, 0, 2.f);
		}
	}
#endif //!UE_BUILD_SHIPPING
}

