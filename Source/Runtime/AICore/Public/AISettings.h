// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Engine/DeveloperSettings.h>

#include "AISettings.generated.h"

UCLASS(config = Game, defaultconfig, MinimalAPI, meta = (DisplayName = "AI Settings"))
class UAISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UAISettings()
		: VillagerClass()
		, NewFamilyEmerganceChance(0.3f)
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "Classes")
	FSoftClassPath VillagerClass;

	UPROPERTY(config, EditDefaultsOnly, Category = "Family Generation", meta = (UIMin = 0, UIMax = 1, ClampMin = 0, ClampMax = 1))
	float NewFamilyEmerganceChance;
};
