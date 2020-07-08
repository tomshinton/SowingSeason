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
	UFUNCTION(Exec, Category = "Building")
	void StartBuildingFromClassPath(const FSoftObjectPath& InClassToBuild);

	/** Update all current construction projects by the given value */
	UFUNCTION(Exec, Category = "Construction")
	void ProgressAllConstructionRequests(const float InAmount);

	/** Enable construction projects to proceed on their own*/
	UFUNCTION(Exec, Category = "Construction")
	void EnableDebugConstruction();

	/** Disable construction projects to proceeding on their own*/
	UFUNCTION(Exec, Category = "Construction")
	void DisableDebugConstruction();

private:

	FTimerHandle DebugConstructionHandle;
};
