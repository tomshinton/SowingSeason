#pragma once

class FFoundationPoint
{
	friend class AGhostRenderer;

public:

	FFoundationPoint(const FVector& InLocation)
		: Location(InLocation)
	{};

	FVector Location;
	FHitResult HitRes;
};