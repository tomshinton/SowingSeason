// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "IdentityInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIdentityInterface : public UInterface
{
	GENERATED_BODY()
};

class IIdentityInterface
{
	GENERATED_BODY()

public:

	virtual void SetFamily(const FGuid& InFamilyID) = 0;
	virtual void SetResidence(const FGuid& InResidenceID) = 0;

	virtual FGuid GetVillagerID() const = 0;
	virtual FGuid GetResidenceID() const = 0;
	virtual FGuid GetFamilyID() const = 0;
};
