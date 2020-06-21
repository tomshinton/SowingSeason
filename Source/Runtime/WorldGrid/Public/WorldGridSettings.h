// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/Engine/DeveloperSettings.h>

#include "WorldGridSettings.generated.h"

UCLASS(config = Game, defaultconfig, MinimalAPI)
class UWorldGridSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UWorldGridSettings()
		: GridCellSize()
		, GridProjectionChannel(ECC_Camera)
		, ReprojectRoundedValues(true)
	{};
	
	FORCEINLINE const FVector GetCellBounds() const
	{
		return FVector(GridCellSize * 0.5, GridCellSize * 0.5, 300.f);
	}

	/** How big is a single grid cell? */
	UPROPERTY(config, EditDefaultsOnly, Category = "Grid Dimensions")
	uint16 GridCellSize;

	/** On what channel do we directly interact with the grid? */
	UPROPERTY(config, EditDefaultsOnly, Category = "Grid Interaction")
	TEnumAsByte<ECollisionChannel> GridProjectionChannel;

	/** When rounding values, do we want to re-project the value onto the Projection Surface? This accounts for non-flat worlds/hills */
	UPROPERTY(config, EditDefaultsOnly, Category = "Grid Interaction")
	bool ReprojectRoundedValues;

	UPROPERTY(config, EditDefaultsOnly, Category = "Ghost | Foundation")
	FSoftObjectPath FoundationCellMesh;

	UPROPERTY(config, EditDefaultsOnly, Category = "Ghost | Foundation")
	FSoftObjectPath ValidFoundationCellMaterial;

	UPROPERTY(config, EditDefaultsOnly, Category = "Ghost | Foundation")
	FSoftObjectPath InValidFoundationCellMaterial;

	UPROPERTY(config, EditDefaultsOnly, Category = "Ghost")
	FSoftObjectPath ValidGhostMaterial;

	UPROPERTY(config, EditDefaultsOnly, Category = "Ghost")
	FSoftObjectPath InvalidGhostMaterial;
};
