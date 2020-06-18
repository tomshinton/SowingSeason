// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagedGameState.h>

#include "SowingSeasonGameState.generated.h"

UCLASS(MinimalAPI)
class ASowingSeasonGameState : public AManagedGameState
{
	GENERATED_BODY()
	
protected:

	void BuildManagerList() override;

private:

	void InitialiseGridLocationManager(UObject& InManager);
	void InitialiseBuildingManager(UObject& InManager);
};
