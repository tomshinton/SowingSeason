#include "Runtime/Build/Public/Ghost/GhostRenderer.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
static TAutoConsoleVariable<int32> CVarDebugGhostRender(TEXT("GhostRenderer.DrawDebugGhost"), 0, TEXT("Draw GhostRenderer debug data"));
#endif //!UE_BUILD_SHIPPING

namespace GhostRendererPrivate
{
	const FName GhostRootComponentName = TEXT("GhostRoot");
	const FName ValidFoundationCellsComponentName = TEXT("ValidFoundationCells");
	const FName InvalidFoundationCellsComponentName = TEXT("InvalidFoundationCells");
}

AGhostRenderer::AGhostRenderer(const FObjectInitializer& ObjectInitializer)
	: GhostRoot(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, GhostRendererPrivate::GhostRootComponentName))
	, ValidFoundationCells(ObjectInitializer.CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(this, GhostRendererPrivate::ValidFoundationCellsComponentName))
	, InvalidFoundationCells(ObjectInitializer.CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(this, GhostRendererPrivate::InvalidFoundationCellsComponentName))
	, ProceduralMeshes()
	, SourceBuildingData(nullptr)
	, BuildingClassCDO(nullptr)
	, ValidGhostMaterial(nullptr)
	, InvalidGhostMaterial(nullptr)
	, LastFoundation()
	, GhostRootLocation(FVector::ZeroVector)
	, GridSettings(GetDefault<UWorldGridSettings>())
	, GridProjection(*this)
	, CachedWorld(nullptr)
	, AssetLoader(MakeUnique<FAsyncLoader>())
{
	RootComponent = GhostRoot;

	ValidFoundationCells->SetupAttachment(RootComponent);
	ValidFoundationCells->bRenderCustomDepth = true;

	InvalidFoundationCells->SetupAttachment(RootComponent);
	ValidFoundationCells->bRenderCustomDepth = true;
}

void AGhostRenderer::BeginPlay()
{
	Super::BeginPlay();
	
	CachedWorld = GetWorld();
	check(CachedWorld != nullptr);

	if (IGridProjectionInterface* GridProjectionInterface = GridProjection.Get())
	{
		GridProjectionInterface->GetOnRoundedPositionChanged().AddLambda([WeakThis = TWeakObjectPtr<AGhostRenderer>(this), this](const FVector& InNewLocation, const FVector& InOldLocation)
		{
			if (WeakThis.IsValid())
			{
				GhostRootLocation = InNewLocation;
			}
		});
	}

	InitialiseAsyncAssets();
}

void AGhostRenderer::InitialiseAsyncAssets()
{
	TWeakObjectPtr<AGhostRenderer> WeakThis = TWeakObjectPtr<AGhostRenderer>(this);

	AssetLoader->RequestLoad<UStaticMesh>(GridSettings->FoundationCellMesh, [WeakThis, this](UStaticMesh& LoadedMesh)
	{
		if (WeakThis.IsValid())
		{
			ValidFoundationCells->SetStaticMesh(&LoadedMesh);
			InvalidFoundationCells->SetStaticMesh(&LoadedMesh);

			AssetLoader->RequestLoad<UMaterialInterface>(GridSettings->InValidFoundationCellMaterial, [WeakThis, this](UMaterialInterface& LoadedMaterial)
			{
				if (WeakThis.IsValid())
				{
					InvalidFoundationCells->SetMaterial(0, &LoadedMaterial);
				}
			});

			AssetLoader->RequestLoad<UMaterialInterface>(GridSettings->ValidFoundationCellMaterial, [WeakThis, this](UMaterialInterface& LoadedMaterial)
			{
				if (WeakThis.IsValid())
				{
					ValidFoundationCells->SetMaterial(0, &LoadedMaterial);
				}
			});
		}
	});
	
	AssetLoader->RequestLoad<UMaterialInterface>(GridSettings->ValidGhostMaterial, [WeakThis](UMaterialInterface& LoadedMaterial)
	{
		if (WeakThis.IsValid())
		{
			WeakThis->ValidGhostMaterial = &LoadedMaterial;
		}
	});

	AssetLoader->RequestLoad<UMaterialInterface>(GridSettings->InvalidGhostMaterial, [WeakThis](UMaterialInterface& LoadedMaterial)
	{
		if (WeakThis.IsValid())
		{
			WeakThis->InvalidGhostMaterial = &LoadedMaterial;
		}
	});
}

void AGhostRenderer::SetGhostInfo(const UBuildingData& InSourceBuildingData)
{
	//Clear the ghost here - this function acts as a re-init function for a new build's build process
	ClearGhost();

	SourceBuildingData = &InSourceBuildingData;

	AssetLoader->RequestLoad<UClass>(InSourceBuildingData.BuildingClass, [WeakThis = TWeakObjectPtr<AGhostRenderer>(this), &InSourceBuildingData, this](UClass& LoadedBuildingClass)
	{
		if (WeakThis.IsValid())
		{
			if (UObject* LoadedCDO = LoadedBuildingClass.GetDefaultObject())
			{
				WeakThis->BuildingClassCDO = LoadedCDO;

				if (InSourceBuildingData.UseProceduralGhostVisual)
				{
					WeakThis->CopyCDOPrimitives(*LoadedCDO);
				}
			}
		}
	});
}

void AGhostRenderer::UpdateRender(const FFoundation& InFoundation)
{
	LastFoundation = InFoundation;

	if (SourceBuildingData->UseProceduralGhostVisual)
	{
		UpdateProceduralMeshes();
	}

	UpdateFoundationRenderer();
}

void AGhostRenderer::UpdateProceduralMeshes()
{
	const bool IsValid = LastFoundation.IsValidFoundation();

	for (UHierarchicalInstancedStaticMeshComponent* ProceduralMesh : ProceduralMeshes)
	{
		ProceduralMesh->SetWorldLocation(GhostRootLocation);

		if (LastFoundation.Rotation.IsSet())
		{
			ProceduralMesh->SetWorldRotation(LastFoundation.Rotation.GetValue());
		}

		if (IsValid)
		{
			ProceduralMesh->SetMaterial(0, ValidGhostMaterial);
		}
		else
		{
			ProceduralMesh->SetMaterial(0, InvalidGhostMaterial);
		}
	}
}

void AGhostRenderer::UpdateFoundationRenderer()
{
	ValidFoundationCells->ClearInstances();
	InvalidFoundationCells->ClearInstances();

	const float PointSize = GridSettings->GridCellSize;
	for (const FFoundationPoint& Point : LastFoundation.Points)
	{
		const FTransform CellTransform = FTransform(FQuat::Identity, Point.Location, FVector(PointSize));

		if (Point.IsValid())
		{
			ValidFoundationCells->AddInstanceWorldSpace(CellTransform);
		}
		else
		{
			InvalidFoundationCells->AddInstanceWorldSpace(CellTransform);
		}
	}
}

void AGhostRenderer::ClearGhost()
{
	for (UHierarchicalInstancedStaticMeshComponent* ProceduralMesh : ProceduralMeshes)
	{
		ProceduralMesh->ClearInstances();
	}

	InvalidFoundationCells->ClearInstances();
	ValidFoundationCells->ClearInstances();
}

void AGhostRenderer::CopyCDOPrimitives(const UObject& InObjectToCopy)
{
	if (const AActor* ObjectAsActor = Cast<AActor>(&InObjectToCopy))
	{
		TArray<UActorComponent*> CDOPrims = ObjectAsActor->GetComponentsByClass(UPrimitiveComponent::StaticClass());

		for (UActorComponent* CDOPrim : CDOPrims)
		{
			if (UStaticMeshComponent* StaticPrim = Cast<UStaticMeshComponent>(CDOPrim))
			{
				const int8 IndexOfMesh = ProceduralMeshes.IndexOfByPredicate([StaticPrim](const UHierarchicalInstancedStaticMeshComponent* Mesh)
				{
					return Mesh->GetStaticMesh() == StaticPrim->GetStaticMesh();
				});

				if (IndexOfMesh != INDEX_NONE)
				{
					ProceduralMeshes[IndexOfMesh]->AddInstance(StaticPrim->GetRelativeTransform());
				}
				else
				{
					UHierarchicalInstancedStaticMeshComponent* NewMesh = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
					NewMesh->RegisterComponent();
					NewMesh->SetWorldLocation(GhostRootLocation);

					NewMesh->SetStaticMesh(StaticPrim->GetStaticMesh());

					ProceduralMeshes.Add(NewMesh);

					NewMesh->AddInstance(StaticPrim->GetRelativeTransform());
				}
			}
		}
	}
}
