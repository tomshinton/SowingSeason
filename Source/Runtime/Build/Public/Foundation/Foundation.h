#pragma once

#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

class FFoundation
{
public:

	FFoundation()
		: Rotation()
		, Points()
	{};

	FFoundation(const TArray<FFoundationPoint>& InPoints)
		: Rotation()
		, Points(InPoints)
	{};

	FFoundation(const TArray<FFoundationPoint>& InPoints, const TOptional<FRotator>& InRotation)
		: Rotation(InRotation)
		, Points(InPoints)
	{};

	TOptional<FRotator> Rotation;
	TArray<FFoundationPoint> Points;
};