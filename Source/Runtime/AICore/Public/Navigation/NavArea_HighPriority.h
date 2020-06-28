// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/NavigationSystem/Public/NavAreas/NavArea.h>

#include "NavArea_HighPriority.generated.h"

UCLASS(BlueprintType)
class UNavArea_HighPriority : public UNavArea
{
	GENERATED_BODY()

public:

	UNavArea_HighPriority()
	{
		DrawColor = FColor(247, 247, 67); // Goldish
		DefaultCost = KINDA_SMALL_NUMBER;
	};
};

