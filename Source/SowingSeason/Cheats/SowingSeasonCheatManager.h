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

	/** Add a selected item to the selected actor's inventory, if they have one */
	UFUNCTION(Exec, Category = "Inventory")
	void AddItemToSelection(const uint8 InAmount, const FName& InName);

	/** Remove a selected item to the selected actor's inventory, if they have one */
	UFUNCTION(Exec, Category = "Inventory")
	void RemoveItemToSelection(const uint8 InAmount, const FName& InName);

private:

	FTimerHandle DebugConstructionHandle;
};
