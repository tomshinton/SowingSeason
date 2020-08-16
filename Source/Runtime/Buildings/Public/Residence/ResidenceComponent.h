#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/Buildings/Public/Residence/ResidenceInterface.h"

#include "ResidenceComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class UResidenceComponent : public UActorComponent
	, public IResidenceInterface
{
	GENERATED_BODY()

public:

	UResidenceComponent();

protected:

	UPROPERTY(EditDefaultsOnly)
	uint8 MaxResidents;

private:

	//IResidenceInterface
	void Occupy(const FGuid& InVillagerID, const FGuid& InFamilyID) override;
	bool IsOccupied() const override;
	bool IsAtCapacity() const override;
	void ClaimForFamily(const FGuid& InClaimingFamily) override;
	bool CanOccupy(const FGuid& InFamilyID) const override;
	FGuid GetResidenceID() const override;
	//IResidenceInterface
	
	FGuid ResidentFamily;
	FGuid ResidenceID;

	TArray<FGuid> ResidentIDs;
};