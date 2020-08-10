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
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "Classes")
	FSoftClassPath VillagerClass;
};
