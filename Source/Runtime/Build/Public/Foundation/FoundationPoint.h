#pragma once

#include "FoundationPoint.generated.h"

USTRUCT()
struct FFoundationPoint
{
	GENERATED_BODY()

public:

	FFoundationPoint()
		: Location()
		, HitRes()
	{};

	FFoundationPoint(const FVector& InLocation)
		: Location(InLocation)
	{};

	bool operator==(const FFoundationPoint& InOtherPoint) const
	{
		return Location != InOtherPoint.Location;
	}

	FVector Location;

	UPROPERTY()
	FHitResult HitRes;
};