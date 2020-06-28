#pragma once

#include "Runtime/Build/Public/FoundationBuilder/FoundationBuilder.h"
#include "Runtime/Build/Public/FoundationBuilder/PointValidator.h"

#include <Runtime/NavigationSystem/Public/NavigationPath.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>

#include "LinearFoundationBuilder.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LinearPointBuilderLog, Log, Log)

UCLASS()
class ULinearFoundationBuilder : public UFoundationBuilder
{
	GENERATED_BODY()

public:

	ULinearFoundationBuilder()
	{};

	void GenerateNewPoints() override
	{
		TArray<FFoundationPoint> Points;

		if (IsCurrentlyBuilding)
		{
			BuildingData->UseNavigationBuilding ? BuildNavigable(Points) : BuildLinear(Points);

			FlushPersistentDebugLines(GetWorld());
			for (FFoundationPoint& Point : Points)
			{
				DrawDebugSphere(GetWorld(), Point.Location, 64.f, 4, FColor::White, true, 20.f);
			}
		}
		else
		{
			Points.Add(CurrentMouseLocation);
		} 

		if (Points.Num() > 0)
		{
			PointValidator->Run(Points, *GetWorld(), [WeakThis = TWeakObjectPtr<ULinearFoundationBuilder>(this)](const TArray<FFoundationPoint>& ValidatedPoints)
			{
				if (WeakThis.IsValid())
				{
					WeakThis->Callback(FFoundation(ValidatedPoints));
				}
			});
		}
	}

	void BuildNavigable(TArray<FFoundationPoint>& OutPoints)
	{
		if (UWorld* World = GetWorld())
		{
			if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World))
			{
				if (UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(World, StartedLocation, CurrentMouseLocation))
				{
					OutPoints.Append(Path->PathPoints);
				}
			}
		}
	}

	void BuildLinear(TArray<FFoundationPoint>& OutPoints)
	{
		const FVector Diff = StartedLocation - CurrentMouseLocation;

		const FVector DiffUnit = FVector::Abs(Diff / GridSettings->GridCellSize);

		const int8 XDir = FMath::Sign(Diff.X);
		const int8 YDir = FMath::Sign(Diff.Y);

		if (DiffUnit.X > DiffUnit.Y)
		{
			//Primary direction
			FVector LastAddedPrimary;
			for (uint8 x = 0; x <= DiffUnit.X; x++)
			{
				const float NewX = StartedLocation.X + (GridSettings->GridCellSize * (x * -XDir));
				LastAddedPrimary = FVector(NewX, StartedLocation.Y, StartedLocation.Z);
				const FFoundationPoint NewPoint(LastAddedPrimary);
				OutPoints.AddUnique(NewPoint);
			}

			//Secondary direction
			for (uint8 y = 0; y <= DiffUnit.Y; y++)
			{
				const float NewY = LastAddedPrimary.Y + (GridSettings->GridCellSize * (y * -YDir));
				const FFoundationPoint NewPoint(FVector(LastAddedPrimary.X, NewY, LastAddedPrimary.Z));
				OutPoints.AddUnique(NewPoint);
			}
		}
		else
		{
			//Primary Direction
			FVector LastAddedPrimary;
			for (uint8 y = 0; y <= DiffUnit.Y; y++)
			{
				const float NewY = StartedLocation.Y + (GridSettings->GridCellSize * (y * -YDir));
				LastAddedPrimary = FVector(StartedLocation.X, NewY, StartedLocation.Z);
				const FFoundationPoint NewPoint(LastAddedPrimary);
				OutPoints.AddUnique(NewPoint);
			}

			//Secondary Direction
			for (uint8 x = 0; x <= DiffUnit.X; x++)
			{
				const float NewX = LastAddedPrimary.X + (GridSettings->GridCellSize * (x * -XDir));
				const FFoundationPoint NewPoint(FVector(NewX, LastAddedPrimary.Y, LastAddedPrimary.Z));
				OutPoints.AddUnique(NewPoint);
			}
		}
	}
};