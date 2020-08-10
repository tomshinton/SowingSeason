// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Build/Public/FoundationBuilder/PointPolicy/PolicyFailReason.h"

class FPointValidationPolicy : public TSharedFromThis<FPointValidationPolicy>
{
public:

	FPointValidationPolicy() {}

	virtual ~FPointValidationPolicy() {}

	virtual bool Run(const FFoundationPoint& InPoint) = 0;
	virtual EPolicyFailReason GetFailReason() const = 0;
};
