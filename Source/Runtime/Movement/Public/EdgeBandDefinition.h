#pragma once

#include "EdgeBandDefinition.generated.h"

USTRUCT(BlueprintType)
struct FScreenEdge
{
	GENERATED_BODY()

	FScreenEdge()
		: XLoc(0.f)
		, YLoc(0.f)
		, Width(0.f)
		, Height(0.f)
		, Strength(0.f)
	{}

	FScreenEdge(const float InXLoc, const float InYLoc, const float InWidth, const float InHeight)
		: XLoc(InXLoc)
		, YLoc(InYLoc)
		, Width(InWidth)
		, Height(InHeight)
		, Strength(0.f)
	{}

public:

	void SetStrength(const float InStrength) { Strength = InStrength; };
	float GetStrength() const { return Strength; };

private:
	float XLoc;
	float YLoc;

	float Width;
	float Height;

	float Strength;
};

USTRUCT(BlueprintType)
struct FEdgeBands
{
	GENERATED_BODY()

public:

	FScreenEdge TopBand;
	FScreenEdge BottomBand;
	FScreenEdge RightBand;
	FScreenEdge LeftBand;
};