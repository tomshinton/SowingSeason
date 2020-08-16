// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/AIIdentity/Public/IdentityInterface.h"

#include "IdentityComponent.generated.h"

class IVillagerInterface;

UCLASS(MinimalAPI)
class UIdentityComponent : public UActorComponent
	, public IIdentityInterface
{
	GENERATED_BODY()

public:

	UIdentityComponent();

private:

	//IIdenityInterface
	void SetFamily(const FGuid& InFamilyID) override { FamilyID = InFamilyID; }
	void SetResidence(const FGuid& InResidenceID) override { ResidenceID = InResidenceID; }
	FGuid GetVillagerID() const override { return VillagerID; }
	FGuid GetResidenceID() const override { return ResidenceID; }
	FGuid GetFamilyID() const override { return FamilyID; }
	//~IIdentityInterface

	FGuid VillagerID;
	FGuid FamilyID;

	FGuid ResidenceID;
};
