// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "BuildInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBuildInterface : public UInterface 
{
	GENERATED_BODY()
};

class IBuildInterface
{
	GENERATED_BODY()

public:

	virtual void StartBuildFromClass(const FSoftObjectPath& InBuildingData) = 0;
};
