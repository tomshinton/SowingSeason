#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/AIIdentity/Public/Family/Types/Family.h"

#include "FamilyGeneratorComponent.generated.h"

UCLASS(MinimalAPI)
class UFamilyGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

	UFamilyGeneratorComponent();

public:

	AIIDENTITY_API FGuid GetFamilyForVillager(const FGuid& RequestingMember);
	AIIDENTITY_API void SetNewFamilyEmerganceChange(const float InNewFamilyChance);

private:

	bool ShouldAddToExistingFamily(FGuid& OutFamilyToAddTo) const;
	void AddMemberToFamily(const FGuid& InFamilyID, const FGuid& InMember);

	TArray<FFamily> Families;

	float NewFamilyChance;
};