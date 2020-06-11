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

	UPROPERTY(EditDefaultsOnly, Category = "Build Mode")
	EBuildMode BuildMode;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClass=Actor), Category = "Build Mode")
	FSoftClassPath BuildingClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	bool UseStaticGhostVisual;

	UPROPERTY(EditDefaultsOnly, meta = (AllowedClass = StaticMesh), Category = "Ghost")
	FSoftObjectPath GhostVisual;

	UPROPERTY(EditDefaultsOnly, Category = "Build Meta")
	bool ShouldCancelBuildPostPlacement;

	UPROPERTY(EditDefaultsOnly)
	FString NameReadable;
};	