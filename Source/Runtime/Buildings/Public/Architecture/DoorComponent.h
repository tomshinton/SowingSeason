// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/Components/SceneComponent.h>

#include "DoorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGarrisonRequested, ACharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUngarrisonRequested, ACharacter*);

UCLASS(meta = (BlueprintSpawnableComponent))
class UDoorComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UDoorComponent(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{};

	FOnGarrisonRequested& GetOnGarrisonRequested() { return OnGarrisonRequested; }
	FOnUngarrisonRequested& GetOnGarrisonUnrequested() { return OnUngarrisonRequested; }

private:

	FOnGarrisonRequested OnGarrisonRequested;
	FOnUngarrisonRequested OnUngarrisonRequested;
};
