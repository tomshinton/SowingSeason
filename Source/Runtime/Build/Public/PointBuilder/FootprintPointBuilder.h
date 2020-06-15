#pragma once

#include "Runtime/Build/Public/PointBuilder/PointBuilder.h"
#include "Runtime/Build/Public/PointBuilder/PointValidator.h"

#include <Runtime/Buildings/Public/FootprintProviderInterface.h>
#include <Runtime/Engine/Classes/Components/BoxComponent.h>

#include "FootprintPointBuilder.generated.h"

DEFINE_LOG_CATEGORY_STATIC(FootprintPointBuilderLog, Log, Log)

namespace FootprintPointBuilderPrivate
{
	const uint8 RawPointCountPadding = 3;
	const float FootprintActualPadding = 100.f;
}

UCLASS()
class UFootprintPointBuilder : public UPointBuilder
{
	GENERATED_BODY()

public:

	UFootprintPointBuilder()
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

			FootprintProxy->SetHiddenInGame(false);
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
						Points.Emplace(NewVector);
					}
				}
			}

			PointValidator = NewObject<UPointValidator>(this);
			PointValidator->Run(Points, *GetWorld(), Callback);
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
		FootprintProxy->AddWorldRotation(FQuat(FRotator(0.f, 45.f, 0.f)));
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
};