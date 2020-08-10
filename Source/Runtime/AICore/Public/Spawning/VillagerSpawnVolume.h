// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "VillagerSpawnVolume.generated.h"

class UBoxComponent;
class USceneComponent;

UCLASS()
class AVillagerSpawnVolume : public AActor
{
	GENERATED_BODY()

public:

	AVillagerSpawnVolume(const FObjectInitializer& ObjectInitialiser);

	UPROPERTY()
	USceneComponent* SpawnerRootComponent;

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* Volume;
};
