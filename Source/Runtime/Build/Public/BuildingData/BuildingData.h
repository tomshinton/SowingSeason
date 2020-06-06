#pragma once

#include <Runtime/Engine/Classes/Engine/DataAsset.h>

#include "Runtime/Build/Public/Ghost/BuildMode.h"

#include "BuildingData.generated.h"

UCLASS()
class UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:

	UBuildingData()
	{};

	UPROPERTY(EditDefaultsOnly, Category = "Build Mode")
	EBuildMode BuildMode;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClass=Actor))
	FSoftClassPath BuildingClass;
};