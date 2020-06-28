#pragma once

#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

class FFoundation
{
public:

	FFoundation()
		: Location()
		, Rotation()
		, Points()
	{};

	FFoundation(const TArray<FFoundationPoint>& InPoints)
		: Location()
		, Rotation()
		, Points(InPoints)
	{};

	FFoundation(const TArray<FFoundationPoint>& InPoints, const TOptional<FRotator>& InRotation, const FVector& InLocation)
		: Location(InLocation)
		, Rotation(InRotation)
		, Points(InPoints)
	{};

	bool IsValidFoundation() const
	{
		for(const FFoundationPoint& Point : Points)
		{
			if (!Point.IsValid())
			{
				return false;
			}
		}

		return true;
	}

	FTransform Transform() const
	{
		return FTransform(Rotation.IsSet() ? Rotation.GetValue() : FRotator::ZeroRotator, Location, FVector(1.f));
	}

	FVector Location;
	TOptional<FRotator> Rotation;

	TArray<FFoundationPoint> Points;
};