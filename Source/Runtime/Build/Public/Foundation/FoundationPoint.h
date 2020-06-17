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

	FVector Location;

	UPROPERTY()
	FHitResult HitRes;
};