#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "GhostRendererInterface.generated.h"

class UBuildingData;

UINTERFACE(MinimalAPI)
class UGhostRendererInterface : public UInterface
{
	GENERATED_BODY()
};

class IGhostRendererInterface
{
	GENERATED_BODY()

public:

	virtual void SetGhostInfo(const UBuildingData& InSourceBuildingData) = 0;
};
