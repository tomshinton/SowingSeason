#include "Runtime/Buildings/Public/Residence/ResidenceComponent.h"

UResidenceComponent::UResidenceComponent()
	: MaxResidents(0)
	, ResidentFamily(FGuid())
	, ResidenceID(FGuid::NewGuid())
	, ResidentIDs()
{

}

bool UResidenceComponent::CanOccupy(const FGuid& InFamilyID) const
{
	return !IsOccupied() ||
		(ResidentFamily == InFamilyID && ResidentIDs.Num() < MaxResidents);
}

FGuid UResidenceComponent::GetResidenceID() const
{
	return ResidenceID;
}

void UResidenceComponent::Occupy(const FGuid& InVillagerID, const FGuid& InFamilyID)
{
	ResidentIDs.AddUnique(InVillagerID);

	if (!ResidentFamily.IsValid())
	{
		ResidentFamily = InFamilyID;
	}
#if !UE_BUILD_SHIPPING
	else
	{
		checkf(ResidentFamily == InFamilyID, TEXT("Cannot occupy a house that is already occupied by another family"));
	}
#endif 
}

bool UResidenceComponent::IsOccupied() const
{
	return ResidentFamily.IsValid();
}

bool UResidenceComponent::IsAtCapacity() const
{
	return ResidentIDs.Num() >= MaxResidents;
}

void UResidenceComponent::ClaimForFamily(const FGuid& InClaimingFamily)
{
	if (!ResidentFamily.IsValid())
	{
		ResidentFamily = InClaimingFamily;
	}
}
