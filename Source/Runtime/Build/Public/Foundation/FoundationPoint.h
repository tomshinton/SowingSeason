#pragma once

class FFoundationPoint
{
	friend class AGhostRenderer;

	FFoundationPoint(const FVector& InLocation)
		: Location(InLocation)
	{};

private:

	FVector Location;
};