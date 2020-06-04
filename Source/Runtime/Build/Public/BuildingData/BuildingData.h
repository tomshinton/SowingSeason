#pragma once

#include <Runtime/Engine/Classes/Engine/DataAsset.h>

#include "BuildingData.generated.h"

UCLASS()
class UBuildingData : public UDataAsset
{
	GENERATED_BODY()

public:

	UBuildingData()
	{};

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClass=Actor))
	FSoftClassPath BuildingClass;
};