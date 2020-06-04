// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/GameModeBase.h>

#include "SowingSeasonGameModeBase.generated.h"

UCLASS(MinimalAPI)
class ASowingSeasonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASowingSeasonGameModeBase(const FObjectInitializer& ObjectInitializer);

};
