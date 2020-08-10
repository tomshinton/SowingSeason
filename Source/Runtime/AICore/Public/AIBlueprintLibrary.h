#pragma once

#include <Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h>

#include "AIBlueprintLibrary.generated.h"

UCLASS()
class UAIBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	static void RequestVillagerSpawn(UObject* InWorldContext, const uint8 InNumToSpawn);
};