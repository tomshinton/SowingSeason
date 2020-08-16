#include "Runtime/AIIdentity/Public/IdentityComponent.h"

UIdentityComponent::UIdentityComponent()
	: VillagerID(FGuid::NewGuid())
	, FamilyID(FGuid())
	, ResidenceID(FGuid())
{

}
