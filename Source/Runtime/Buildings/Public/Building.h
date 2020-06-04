#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Building.generated.h"

class USceneComponent;

UCLASS()
class ABuilding : public AActor
{
	GENERATED_BODY()

public:

	ABuilding(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	USceneComponent* BuildingRoot;
};