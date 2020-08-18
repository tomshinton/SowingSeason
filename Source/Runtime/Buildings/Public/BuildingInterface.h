// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "BuildingInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBuildingInterface : public UInterface
{
	GENERATED_BODY()
};

class IBuildingInterface
{
	GENERATED_BODY()

public:

	virtual FGuid GetBuildingID() const = 0;
};
