#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "GridProjectionInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRoundedPositionChanged, const FVector& /*NewPosition*/, const FVector& /*OldPosition*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPositionChanged, const FVector& /*NewPosition*/, const FVector& /*OldPosition*/);

UINTERFACE(MinimalAPI)
class UGridProjectionInterface : public UInterface
{
	GENERATED_BODY()
};

class IGridProjectionInterface
{
	GENERATED_BODY()

public:

	virtual const FVector GetPositionUnderMouse() const = 0;
	virtual const FVector GetRoundedPositionUnderMouse() const = 0;

	virtual FOnRoundedPositionChanged& GetOnRoundedPositionChanged() = 0;
	virtual FOnPositionChanged& GetOnPositionChanged() = 0;
};
