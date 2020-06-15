#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "GhostRendererInterface.generated.h"

class UBuildingData;
class FFoundationPoint;

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
	virtual void UpdateRender(const TArray<FFoundationPoint>& InPoints) = 0;
	virtual void ClearGhost() = 0;
};
