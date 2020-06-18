#pragma once

#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

class FFoundation
{
public:

	FFoundation()
		: Points()
	{};

	FFoundation(const TArray<FFoundationPoint>& InPoints)
		: Points(InPoints)
	{};

	void PushPoint(const FFoundationPoint& InNewPoint) { Points.AddUnique(InNewPoint); }

private:

	TArray<FFoundationPoint> Points;
};