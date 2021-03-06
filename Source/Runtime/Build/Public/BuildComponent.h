// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/Build/Public/BuildInterface.h"
#include "Runtime/Build/Public/Foundation/Foundation.h"

#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

#include "BuildComponent.generated.h"

class AGhostRenderer;
class IGridProjectionInterface;
class IGhostRendererInterface;
class UBuildingData;
class UFoundationBuilder;

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
	void SetupComponentInputBindings(UInputComponent& PlayerInputComponent) override;

	void InitialiseGhost(UClass& InGhostClass);

	//IBuildInterface
	void StartBuildFromClass(const FSoftObjectPath& InBuildingData) override;
	//~IBuildInterface

	void StartBuild();
	void EndBuild();
	void CancelBuild();

	void RotateBuild();

	void OnFoundationGenerated(const FFoundation& InNewPoints);

	bool IsBuildingValid() const;

	void ResetFoundationBuilder();

	TUniquePtr<FAsyncLoader> AsyncLoader;

	UPROPERTY()
	const UBuildingData* BuildingData;

	UPROPERTY()
	TSubclassOf<AGhostRenderer> GhostClass;

	TWeakInterfacePtr<IGhostRendererInterface> GhostRenderer;

	UPROPERTY(Transient)
	UFoundationBuilder* CurrentFoundationBuilder;

	FFoundation LastFoundation;
};
