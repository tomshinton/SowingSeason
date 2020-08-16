// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "ResidenceInterface.generated.h"

UINTERFACE(MinimalAPI)
class UResidenceInterface : public UInterface
{
	GENERATED_BODY()
};

class IResidenceInterface
{
	GENERATED_BODY()

public:

	virtual bool IsOccupied() const = 0;
	virtual bool IsAtCapacity() const = 0;
	virtual void Occupy(const FGuid& InVillagerID, const FGuid& InFamilyID) = 0;
	virtual bool CanOccupy(const FGuid& InFamilyID) const = 0;
	virtual void ClaimForFamily(const FGuid& InClaimingFamily) = 0;

	virtual FGuid GetResidenceID() const = 0;
};
