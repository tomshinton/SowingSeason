// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/AIModule/Classes/AIController.h>

#include "VillagerController.generated.h"

UCLASS(abstract, Blueprintable, MinimalAPI)
class AVillagerController : public AAIController
{
	GENERATED_BODY()

public:

	AVillagerController(const FObjectInitializer& ObjectInitialiser);
};