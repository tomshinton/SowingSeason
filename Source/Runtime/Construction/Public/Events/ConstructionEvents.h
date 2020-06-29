#pragma once

#include "ConstructionEvents.generated.h"

USTRUCT()
struct FUpdateConstructionRequest
{
	GENERATED_BODY()

public:

	FUpdateConstructionRequest()
		: ID(FGuid())
		, Amount(0.f)
	{};

	FUpdateConstructionRequest(const FGuid& InID, const float InAmount)
		: ID(InID)
		, Amount(InAmount)
	{};

#if !UE_BUILD_SHIPPING
	//Debug constructor to allow for all buildings to be updated, regardless of ID.  Allows for debugging without builders being involved
	FUpdateConstructionRequest(const float InAmount)
		: ID(FGuid())
		, Amount(InAmount)
	{};
#endif //!UE_BUILD_SHIPPING

	FGuid ID;
	float Amount;
};