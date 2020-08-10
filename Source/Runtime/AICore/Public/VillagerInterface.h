#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "VillagerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UVillagerInterface : public UInterface
{
	GENERATED_BODY()
};

class IVillagerInterface
{
	GENERATED_BODY()

public:

	virtual void RequestVillagerSpawn() = 0;
};
