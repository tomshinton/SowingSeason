#include "Runtime/Build/Public/Ghost/GhostRenderer.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>
#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#if !UE_BUILD_SHIPPING
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
static TAutoConsoleVariable<int32> CVarDebugGhostRender(TEXT("GhostRenderer.DrawDebugGhost"), 0, TEXT("Draw GhostRenderer debug data"));
#endif //!UE_BUILD_SHIPPING

AGhostRenderer::AGhostRenderer(const FObjectInitializer& ObjectInitializer)
	: SourceBuildingData(nullptr)
	, BuildingClassCDO(nullptr)
	, LastRenderedPoints()
	, GhostRoot(FVector::ZeroVector)
	, GridSettings(GetDefault<UWorldGridSettings>())
	, GridProjection(*this)
	, CachedWorld(nullptr)
	, AssetLoader(MakeUnique<FAsyncLoader>())
{

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
				GhostRoot = InNewLocation;
			}
		});
	}
}

void AGhostRenderer::SetGhostInfo(const UBuildingData& InSourceBuildingData)
{
	SourceBuildingData = &InSourceBuildingData;

	AssetLoader->RequestLoad<UClass>(InSourceBuildingData.BuildingClass, [WeakThis = TWeakObjectPtr<AGhostRenderer>(this), &InSourceBuildingData, this](UClass& LoadedBuildingClass)
	{
		if (WeakThis.IsValid())
		{
			if (UObject* LoadedCDO = LoadedBuildingClass.GetDefaultObject())
			{
				WeakThis->BuildingClassCDO = LoadedCDO;
				WeakThis->CopyCDOPrimitives(*LoadedCDO);

				switch (InSourceBuildingData.BuildMode)
				{
				case EBuildMode::FireAndForget:
					InitFireAndForgetGhost();
					break;
				}
			}
		}
	});
}

void AGhostRenderer::UpdateRender(const TArray<FFoundationPoint>& InPoints)
{
	LastRenderedPoints = InPoints;

	for(UHierarchicalInstancedStaticMeshComponent* ProceduralMesh : ProceduralMeshes)
	{
		ProceduralMesh->SetWorldLocation(GhostRoot);
	}

#if !UE_BUILD_SHIPPING
	if (CVarDebugGhostRender.GetValueOnAnyThread())
	{
		FlushPersistentDebugLines(CachedWorld);
		for (const FFoundationPoint& Point : InPoints)
		{
			DrawDebugSphere(CachedWorld, Point.Location, GridSettings->GridCellSize * .5, 4.f, FColor::Green, true, -1, 0, 2.f);
		}
	}
#endif //!UE_BUILD_SHIPPING
}

void AGhostRenderer::ClearGhost()
{
	for (UHierarchicalInstancedStaticMeshComponent* ProceduralMesh : ProceduralMeshes)
	{
		ProceduralMesh->ClearInstances();
	}

	ProceduralMeshes.Empty();
}

void AGhostRenderer::InitFireAndForgetGhost()
{

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
					NewMesh->SetWorldLocation(GhostRoot);

					NewMesh->SetStaticMesh(StaticPrim->GetStaticMesh());

					ProceduralMeshes.Add(NewMesh);

					NewMesh->AddInstance(StaticPrim->GetRelativeTransform());
				}
			}
		}
	}
}
