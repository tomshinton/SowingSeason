// Sowing Season - Tom Shinton 2020

#include "Runtime/WorldGrid/Public/GridProjection/GridProjectionManager.h"
#include "Runtime/WorldGrid/Public/WorldGridSettings.h"

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#endif //!UE_BUILD_SHIPPING

DEFINE_LOG_CATEGORY_STATIC(GridLocationManagerLog, Log, Log)

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarDrawDebugGridLocations(TEXT("GridLocations.DrawDebugGridLocations"), 0, TEXT("Draw where the GridLocationManager is currently considering 'Under the mouse', both rounded and raw"));
#endif //!UE_BUILD_SHIPPING

namespace GridLocationManagerPrivate
{
    const float UpdateRate = 1.f / 30.f; //About 30 times a second
    const float TraceDepth = 50000.f;
}

UGridProjectionManager::UGridProjectionManager()
    : CachedWorld(nullptr)
    , LocalPlayerController(nullptr)
    , WorldGridSettings(GetDefault<UWorldGridSettings>())
    , RoundedPosition(FVector::ZeroVector)
    , Position(FVector::ZeroVector)
    , UpdatePositionHandle()
    , OnRoundedPositionChanged()
    , OnPositionChanged()
    , TraceParams()
{}

void UGridProjectionManager::Init(const UWorld& InWorld)
{
	if (WorldGridSettings != nullptr)
	{
		CachedWorld = &InWorld;
		InWorld.GetTimerManager().SetTimer(UpdatePositionHandle, this, &UGridProjectionManager::QueryScene, GridLocationManagerPrivate::UpdateRate, true, GridLocationManagerPrivate::UpdateRate);
	}
}

const FVector UGridProjectionManager::GetPositionUnderMouse() const
{
    return Position;
}

const FVector UGridProjectionManager::GetRoundedPositionUnderMouse() const
{
	return RoundedPosition;
}

FOnRoundedPositionChanged& UGridProjectionManager::GetOnRoundedPositionChanged()
{
    return OnRoundedPositionChanged;
}

FOnPositionChanged& UGridProjectionManager::GetOnPositionChanged()
{
    return OnPositionChanged;
}

void UGridProjectionManager::QueryScene()
{
    if(!LocalPlayerController)
    {
        //We've not yet got a locally possessed pawn.  Bail out now, we'll get one when it's avaliable
        LocalPlayerController = CachedWorld->GetFirstPlayerController();
        return;
    }

    FVector CurrPosUnderMouse;
    FVector CurrDirUnderMouse;
    LocalPlayerController->DeprojectMousePositionToWorld(CurrPosUnderMouse, CurrDirUnderMouse);
    const FVector ProjectedEnd = CurrPosUnderMouse + (CurrDirUnderMouse * GridLocationManagerPrivate::TraceDepth);

    FHitResult HitRes;
    CachedWorld->LineTraceSingleByChannel(HitRes, CurrPosUnderMouse, ProjectedEnd, WorldGridSettings->GridProjectionChannel, TraceParams);

#if !UE_BUILD_SHIPPING
	if (CVarDrawDebugGridLocations.GetValueOnAnyThread())
	{
        GEngine->AddOnScreenDebugMessage(1, 1, FColor::White, FString::Printf(TEXT("Cursor world position: %s"), *CurrPosUnderMouse.ToString()));
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::White, FString::Printf(TEXT("Projecting grid to: %s"), *ProjectedEnd.ToString()));
	}
#endif //!UE_BUILD_SHIPPING

    if (HitRes.bBlockingHit)
    {
        ProcessHitResult(HitRes);
    }
}

void UGridProjectionManager::ProcessHitResult(const FHitResult& InHitResult)
{
    if (AActor* ProjectionSurface = InHitResult.Actor.Get())
    {
        ProcessNewPosition(InHitResult.ImpactPoint);

        const FVector RoundedRaw = FMath::RoundVectorToNearestInt(Position, WorldGridSettings->GridCellSize);

        if (WorldGridSettings->ReprojectRoundedValues)
        {   
            FHitResult ReprojectionResult;
            const FVector ReprojectionOffset = FVector(0.f, 0.f, GridLocationManagerPrivate::TraceDepth);
            const FVector ReprojectionStart = RoundedRaw + ReprojectionOffset;
            const FVector ReprojectionEnd = RoundedRaw - ReprojectionOffset;

			CachedWorld->LineTraceSingleByChannel(ReprojectionResult, ReprojectionStart, ReprojectionEnd, WorldGridSettings->GridProjectionChannel, TraceParams);

            if (ReprojectionResult.bBlockingHit)
            {
                ProcessNewRoundedPosition(FVector(RoundedRaw.X, RoundedRaw.Y, ReprojectionResult.ImpactPoint.Z));
            }
        }
        else
        {
            ProcessNewRoundedPosition(FVector(RoundedRaw.X, RoundedRaw.Y, Position.Z));
        }

#if !UE_BUILD_SHIPPING
    if (CVarDrawDebugGridLocations.GetValueOnAnyThread())
    {
        FlushPersistentDebugLines(CachedWorld);
        FlushDebugStrings(CachedWorld);

		GEngine->AddOnScreenDebugMessage(3, 1, FColorList::HunterGreen, FString::Printf(TEXT("Position: %s"), *Position.ToString()));
		GEngine->AddOnScreenDebugMessage(4, 1, FColorList::HunterGreen, FString::Printf(TEXT("Rounded Pos: %s"), *RoundedPosition.ToString()));

        DrawDebugSphere(CachedWorld, Position, 12.f, 5.f, FColorList::SpringGreen, false, 10.f, 0, 5.f);
        DrawDebugString(CachedWorld, Position, TEXT("Pos under mouse"), nullptr, FColor::White, 10.f, false);

		DrawDebugSphere(CachedWorld, RoundedPosition, 12.f, 5.f, FColorList::BlueViolet, false, 10.f, 0, 5.f);
		DrawDebugString(CachedWorld, RoundedPosition, TEXT("Rounded Pos under mouse"), nullptr, FColor::White, 10.f, false);
    }
#endif //!UE_BUILD_SHIPPING
    }
}

void UGridProjectionManager::ProcessNewPosition(const FVector& InNewPosition)
{
    if (!InNewPosition.Equals(Position))
    {
        const FVector OldPos = Position;
	    Position = InNewPosition;
        OnPositionChanged.Broadcast(Position, OldPos);
    }
}

void UGridProjectionManager::ProcessNewRoundedPosition(const FVector& InNewRoundedPosition)
{
	if (!InNewRoundedPosition.Equals(RoundedPosition))
	{
		const FVector OldPos = RoundedPosition;
        RoundedPosition = InNewRoundedPosition;
		OnRoundedPositionChanged.Broadcast(InNewRoundedPosition, OldPos);
	}
}
