#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/Family/Public/Types/Family.h"

#include "FamilyGeneratorComponent.generated.h"

UCLASS(MinimalAPI)
class UFamilyGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

	UFamilyGeneratorComponent();

public:

	FAMILY_API FGuid GetFamilyForVillager(const FGuid& RequestingMember);

private:

	bool ShouldAddToExistingFamily(FGuid& OutFamilyToAddTo) const;
	void AddMemberToFamily(const FGuid& InFamilyID, const FGuid& InMember);

	TArray<FFamily> Families;
};