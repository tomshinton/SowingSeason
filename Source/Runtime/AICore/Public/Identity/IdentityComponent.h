// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

#include "IdentityComponent.generated.h"

class IVillagerInterface;

UCLASS()
class UIdentityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UIdentityComponent();

	void BeginPlay() override;

private:

	void GetFamily();

	TManagerPtr<IVillagerInterface> VillagerManager;

	FGuid VillagerID;
	FGuid FamilyID;
};
