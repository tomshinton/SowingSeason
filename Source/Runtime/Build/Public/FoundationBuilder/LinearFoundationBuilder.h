#pragma once

#include "Runtime/Build/Public/FoundationBuilder/FoundationBuilder.h"
#include "Runtime/Build/Public/FoundationBuilder/PointValidator.h"

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
			const FVector Diff = StartedLocation - CurrentMouseLocation;
			const FVector DiffUnit = FVector::Abs(Diff / GridSettings->GridCellSize);

			const int8 XDir = FMath::Sign(Diff.X);
			const int8 YDir = FMath::Sign(Diff.Y);

			if (DiffUnit.X > DiffUnit.Y)
			{
				for (uint8 x = 0; x <= DiffUnit.X; x++)
				{
					const float NewX = StartedLocation.X + (GridSettings->GridCellSize * (x * -XDir));
					const FFoundationPoint NewPoint(FVector(NewX, StartedLocation.Y, StartedLocation.Z));
					Points.AddUnique(NewPoint);
				}
			}
			else
			{
				for (uint8 y = 0; y <= DiffUnit.Y; y++)
				{
					const float NewY = StartedLocation.Y + (GridSettings->GridCellSize * (y * -YDir));
					const FFoundationPoint NewPoint(FVector(StartedLocation.X, NewY, StartedLocation.Z));
					Points.AddUnique(NewPoint);
				}
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
};