// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Engine/LevelScriptActor.h>

#include "SowingSeasonLevel.generated.h"

class ULevelMaskComponent;

UCLASS(MinimalAPI)
class ASowingSeasonLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	ASowingSeasonLevel(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly)
	ULevelMaskComponent* MaskComponent;
};
