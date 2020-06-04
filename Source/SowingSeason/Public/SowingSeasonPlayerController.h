// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/PlayerController.h>

#include "SowingSeasonPlayerController.generated.h"

UCLASS(MinimalAPI)
class  ASowingSeasonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASowingSeasonPlayerController(const FObjectInitializer& ObjectInitializer);

private:
	void BeginPlay() override;
};
