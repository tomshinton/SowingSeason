// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "GarrisonInterface.generated.h"

class USceneComponent;

UINTERFACE(MinimalAPI)
class UGarrisonInterface : public UInterface
{
	GENERATED_BODY()
};

class IGarrisonInterface
{
	GENERATED_BODY()

public:

	virtual bool RequestGarrison(AActor& InRequestingActor) = 0;
	virtual bool RequestUngarrison(AActor& InRequestingActor) = 0;

	virtual bool HasDoors() const = 0;
	virtual USceneComponent& GetRandomDoor() const = 0;
};
