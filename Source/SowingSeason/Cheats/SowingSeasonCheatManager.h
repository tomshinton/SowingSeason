// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/CheatManager.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

#include "SowingSeasonCheatManager.generated.h"

class IBuildInterface;

UCLASS()
class USowingSeasonCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:

	USowingSeasonCheatManager();

	/** Trigger building of a class from the given path */
	UFUNCTION(Exec)
	void StartBuildingFromClassPath(const FSoftObjectPath& InClassToBuild);
};
