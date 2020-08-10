#include "Runtime/AICore/Public/Identity/IdentityComponent.h"

#include "Runtime/AICore/Public/VillagerInterface.h"

UIdentityComponent::UIdentityComponent()
	: VillagerManager(*this)
	, VillagerID(FGuid::NewGuid())
	, FamilyID(FGuid())
{

}

void UIdentityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetFamily();
}

void UIdentityComponent::GetFamily()
{
	if (IVillagerInterface* VillagerInterface = VillagerManager.Get())
	{
		FamilyID = VillagerInterface->RequestFamily(VillagerID);
	}
}
