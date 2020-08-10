// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/PointValidationPolicy.h"

class FTooSteepPointPolicy : public FPointValidationPolicy
{
public:

	FTooSteepPointPolicy() {};

	FORCEINLINE bool Run(const FFoundationPoint& InPoint) override
	{
		return (InPoint.HitRes.Normal | FVector(0.f, 0.f, 1.f)) > 0.9f;
	}

	EPolicyFailReason GetFailReason() const override
	{
		return EPolicyFailReason::TooSteep;
	}
};
