// Sowing Season - Tom Shinton 2020

#pragma once

#include "FoundationPoint.generated.h"

enum class EPolicyFailReason : uint8;

USTRUCT()
struct FFoundationPoint
{
	GENERATED_BODY()

public:

	FFoundationPoint()
		: Location()
		, HitRes()
		, PolicyFailReasons()
	{};

	FFoundationPoint(const FVector& InLocation)
		: Location(InLocation)
		, HitRes()
		, PolicyFailReasons()
	{};

	bool operator==(const FFoundationPoint& InOtherPoint) const
	{
		return Location == InOtherPoint.Location;
	}

	bool IsValid() const 
	{
		return PolicyFailReasons.Num() == 0;
	}

	FVector Location;

	UPROPERTY()
	FHitResult HitRes;

	TArray<EPolicyFailReason> PolicyFailReasons;
};
