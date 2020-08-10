#include "Runtime/Family/Public/FamilyGeneratorComponent.h"
#include "Runtime/Family/Public/Types/Family.h"

DEFINE_LOG_CATEGORY_STATIC(FamilyGeneratorLog, Log, All)

namespace FamilyGeneratorPrivate
{
	const float NewFamilyChance = 0.3f;
}

UFamilyGeneratorComponent::UFamilyGeneratorComponent()
{

}

FGuid UFamilyGeneratorComponent::GetFamilyForVillager(const FGuid& RequestingMember)
{
	FGuid FamilyToAddTo;
	if(ShouldAddToExistingFamily(FamilyToAddTo))
	{
		UE_LOG(FamilyGeneratorLog, Log, TEXT("Added Villager to existing family"));
		AddMemberToFamily(FamilyToAddTo, RequestingMember);
		return FamilyToAddTo;
	}
	else
	{
		const int32 IndexOfNewFamily = Families.Emplace(RequestingMember);

		UE_LOG(FamilyGeneratorLog, Log, TEXT("New family created - now %i"), Families.Num());
		return Families[IndexOfNewFamily].GetID();
	};
}

bool UFamilyGeneratorComponent::ShouldAddToExistingFamily(FGuid& OutFamilyToAddTo) const
{
	if (Families.Num() == 0 || FMath::FRand() <= FamilyGeneratorPrivate::NewFamilyChance)
	{
		//We dont have any families currently, so lets create one for this pawn
		return false;
	}
	else
	{
		//Find the family with the fewest members, and add to that one
		int32 CurrentLeast = 10000;

		for (const FFamily& Family : Families)
		{
			const uint8 CurrCount = Family.Count();
			if (CurrCount < CurrentLeast)
			{
				OutFamilyToAddTo = Family.GetID();
				CurrentLeast = CurrCount;
			}
		}

		return true;
	}
}

void UFamilyGeneratorComponent::AddMemberToFamily(const FGuid& InFamilyID, const FGuid& InMember)
{
	FFamily* FoundFamily = Families.FindByPredicate([InFamilyID](const FFamily & Family)
	{
		return Family.GetID() == InFamilyID;
	});

	if (FoundFamily != nullptr)
	{
		FoundFamily->AddMemberToFamily(InMember);
	}
}
