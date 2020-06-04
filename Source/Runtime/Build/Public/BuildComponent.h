// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "Runtime/Build/Public/BuildInterface.h"

#include <Runtime/Core/Public/Templates/UniquePtr.h>
#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>
#include <Runtime/Engine/Classes/Components/ActorComponent.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

#include "BuildComponent.generated.h"

class ABuildingGhost;
class IGridProjectionInterface;
class IGhostInterface;
class UBuildingData;

UCLASS( MinimalAPI, ClassGroup=(Build))
class UBuildComponent : public UActorComponent
	, public IBuildInterface
{
	GENERATED_BODY()

public:	

	UBuildComponent();

protected:

	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath SoftGhostClass;

private:

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//IBuildInterface
	void StartBuildFromClass(const FSoftObjectPath& InBuildingData) override;
	void TryCancelBuild();
	//~IBuildInterface

	void OnRoundedPositionChanged(const FVector& InNewPosition, const FVector& InOldPosition);

	TUniquePtr<FAsyncLoader> AsyncLoader;

	UPROPERTY()
	const UBuildingData* CurrentBuildData;

	UPROPERTY()
	TSubclassOf<ABuildingGhost> GhostClass;

	TWeakInterfacePtr<IGhostInterface> CurrentGhost;
	TManagerPtr<IGridProjectionInterface> GridProjectionInterface;
};
