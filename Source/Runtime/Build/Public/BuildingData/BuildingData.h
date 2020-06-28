#pragma once

#include <Runtime/Engine/Classes/Engine/DataAsset.h>

#include "Runtime/Build/Public/Ghost/BuildMode.h"

#include "BuildingData.generated.h"

class FPointBuilder;

UCLASS()
class UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:

	UBuildingData();

	/** How does this building generate it's foundation? */
	UPROPERTY(EditDefaultsOnly, Category = "Build Mode")
	EBuildMode BuildMode;

	/** The class of the building to actually spawn */
	UPROPERTY(EditDefaultsOnly, meta=(AllowedClass=Actor), Category = "Build Mode")
	FSoftClassPath BuildingClass;

	/** When dragging a foundation out, do the points orientate to the navmesh? useful for stuff like roads and walls, that should go AROUND existing obstacles instead of through */
	UPROPERTY(EditDefaultsOnly, meta = (AllowedClass = Actor), Category = "Build Mode")
	bool UseNavigationBuilding;

	/** Does this building provide a working ghost based off of the spawned classes's primitives? Ie Can we see an outline of the building we're going to build inside the ghost? */
	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	bool UseProceduralGhostVisual;

	/** Whether we immediately cancel this building once there's been an attempt to build it.  Toggle off for stuff like roads/walls, as they tend to get built in quick succession */
	UPROPERTY(EditDefaultsOnly, Category = "Build Meta")
	bool ShouldCancelBuildPostPlacement;

	/** Human readable name for this building, useful for debugging*/
	UPROPERTY(EditDefaultsOnly)
	FString NameReadable;
};	