#pragma once

#include "Runtime/Build/Public/FoundationBuilder/FoundationBuilder.h"
#include "Runtime/Build/Public/FoundationBuilder/PointValidator.h"

#include <Runtime/Buildings/Public/FootprintProviderInterface.h>
#include <Runtime/Engine/Classes/Components/BoxComponent.h>

#include "FootprintFoundationBuilder.generated.h"

DEFINE_LOG_CATEGORY_STATIC(FootprintPointBuilderLog, Log, Log)

namespace FootprintFoundationBuilderPrivate
{
	const FRotator RotationRate = FRotator(0.f, 45.f, 0.f);
}

namespace FootprintPointBuilderPrivate
{
	const uint8 RawPointCountPadding = 3;
	const float FootprintActualPadding = 100.f;
}

UCLASS()
class UFootprintFoundationBuilder : public UFoundationBuilder
{
	GENERATED_BODY()

public:

	UFootprintFoundationBuilder()
		: FootprintProxy(nullptr)
		, BuildingBounds()
		, RawPointCountExtent(0)
		, PointValidator(nullptr)
	{};

	void InitialiseFootprintPointBuilder()
	{
		if (IFootprintProvider* FootprintProvider = Cast<IFootprintProvider>(BuildingCDO))
		{
			const FBoxSphereBounds Bounds = FootprintProvider->GetFootprintReference().Bounds;
			BuildingBounds = Bounds.SphereRadius;

			RawPointCountExtent = (uint8)(BuildingBounds.GetValue() / (GridSettings->GridCellSize * 0.5)) + FootprintPointBuilderPrivate::RawPointCountPadding;
			RawPointCountExtent *= 0.5f;

			FootprintProxy = NewObject<UBoxComponent>(GetOuter());
			FootprintProxy->SetBoxExtent(Bounds.BoxExtent * FVector(1.f, 1.f, 100.f));
			FootprintProxy->RegisterComponent();
		}
	}

	void GenerateNewPoints() override
	{
		if (!BuildingBounds.IsSet())
		{
			InitialiseFootprintPointBuilder();
		}

		if (BuildingBounds.IsSet())
		{
			FootprintProxy->SetWorldLocation(CurrentMouseLocation);

			TArray<FFoundationPoint> Points;
			for (int8 x = -RawPointCountExtent; x <= RawPointCountExtent; ++x)
			{
				for (int8 y = -RawPointCountExtent; y <= RawPointCountExtent; ++y)
				{
					const FVector NewVector = CurrentMouseLocation + FVector(GridSettings->GridCellSize * x, GridSettings->GridCellSize * y, 0.f);

					if (IsPointOnFootprint(NewVector))
					{
						Points.AddUnique(NewVector);
					}
				}
			}

			PointValidator = NewObject<UPointValidator>(this);
			PointValidator->Run(Points, *GetWorld(), [WeakThis = TWeakObjectPtr<UFootprintFoundationBuilder>(this)](const TArray<FFoundationPoint>& ValidatedPoints)
			{
				if (WeakThis.IsValid())
				{
					WeakThis->Callback(FFoundation(ValidatedPoints, WeakThis->CurrentRotation, WeakThis->CurrentMouseLocation));
				}
			});
		}
	}

	void Teardown() override
	{
		Super::Teardown();

		FootprintProxy->UnregisterComponent();
		FootprintProxy = nullptr;

		PointValidator->Stop();
		PointValidator = nullptr;
	}

	void RotateBuild() override
	{
		FootprintProxy->AddWorldRotation(FQuat(FootprintFoundationBuilderPrivate::RotationRate));
		CurrentRotation += FootprintFoundationBuilderPrivate::RotationRate;

		GenerateNewPoints();
	}

private:

	bool IsPointOnFootprint(const FVector& InPoint)
	{
		FVector ClostestPoint;
		return FootprintProxy->GetDistanceToCollision(InPoint, ClostestPoint) <= FootprintPointBuilderPrivate::FootprintActualPadding;
	};

	UPROPERTY()
	UBoxComponent* FootprintProxy;

	TOptional<float> BuildingBounds;
	uint8 RawPointCountExtent;

	UPROPERTY()
	UPointValidator* PointValidator;

	FRotator CurrentRotation;
};