#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Build/Public/Ghost/GhostRendererInterface.h"
#include "Runtime/Build/Public/Foundation/FoundationPoint.h"

#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/Engine/Classes/Components/HierarchicalInstancedStaticMeshComponent.h>
#include <Runtime/Engine/Classes/GameFramework/ManagerFramework/ManagerPtr.h>

#include "GhostRenderer.generated.h"

class IGridProjectionInterface;
class UWorldGridSettings;

UCLASS()
class AGhostRenderer : public AActor
	, public IGhostRendererInterface
{
	GENERATED_BODY()

public:

	AGhostRenderer(const FObjectInitializer& ObjectInitializer);

private:

	void BeginPlay() override;

	//IGhostInterface
	virtual void SetGhostInfo(const UBuildingData& InSourceData) override;
	virtual void UpdateRender(const FFoundation& InFoundation) override;
	virtual void ClearGhost() override;
	//~IGhostInterface

	void InitialiseSingleGhost();

	void CopyCDOPrimitives(const UObject& InObjectToCopy);

	UPROPERTY(Transient)
	const UBuildingData* SourceBuildingData;

	UPROPERTY(Transient)
	UObject* BuildingClassCDO;

	FFoundation LastFoundation;

	FVector GhostRoot;

	UPROPERTY()
	const UWorldGridSettings* GridSettings;

	TManagerPtr<IGridProjectionInterface> GridProjection;

	UPROPERTY()
	UWorld* CachedWorld;

	UPROPERTY()
	TArray<UHierarchicalInstancedStaticMeshComponent*> ProceduralMeshes;

	TUniquePtr<FAsyncLoader> AssetLoader;
};