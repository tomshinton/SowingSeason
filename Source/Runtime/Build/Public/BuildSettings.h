// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Engine/DeveloperSettings.h>

#include "BuildSettings.generated.h"

UCLASS(config = Game, defaultconfig, MinimalAPI)
class UBuildSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UBuildSettings()
	{};
	
	/** What channel to we check for when seeing if a point is occluded? */
	UPROPERTY(config, EditDefaultsOnly, Category = "Point Validation")
	TEnumAsByte<ECollisionChannel> PlacementOverlapChannel;

	/** ConstructionSite BP Class */
	UPROPERTY(config, EditDefaultsOnly, Category = "Construction")
	FSoftClassPath ConstructionSiteClass;
};
