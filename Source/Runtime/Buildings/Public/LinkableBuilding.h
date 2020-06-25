#pragma once

#include "Runtime/Buildings/Public/Building.h"

#include "LinkableBuilding.generated.h"

UCLASS(MinimalAPI)
class ALinkableBuilding : public ABuilding

{
	GENERATED_BODY()

public:

	ALinkableBuilding(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{};
};