#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "GhostInterface.generated.h"

class UBuildingData;

UINTERFACE(MinimalAPI)
class UGhostInterface : public UInterface
{
	GENERATED_BODY()
};

class IGhostInterface
{
	GENERATED_BODY()

public:

	virtual void SetGhostInfo(const UBuildingData& InSourceBuildingData) = 0;
	virtual void DestroyGhost() = 0;
};
