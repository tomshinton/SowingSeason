#include "Runtime/Construction/Public/ConstructionSite/ConstructionSite.h"
#include "Runtime/Construction/Public/ConstructionRequest.h"

#include <Runtime/WorldGrid/Public/WorldGridSettings.h>

#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include <Runtime/Engine/Classes/Components/SceneComponent.h>

namespace ConstructionSitePrivate
{
	const FName ConstructionSiteRoot = TEXT("ConstructionSiteRoot");
	const FName SiteCollision = TEXT("SiteCollision");
}

AConstructionSite::AConstructionSite(const FObjectInitializer& ObjectInitializer)
	: Root(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, ConstructionSitePrivate::ConstructionSiteRoot))
	, SiteCollision(ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, ConstructionSitePrivate::SiteCollision))
	, DecalMaterialClass()
	, SourceRequest()
	, SpawnedDecals()
	, GridSettings(GetDefault<UWorldGridSettings>())
	, AssetLoader(MakeUnique<FAsyncLoader>())
{
	SetRootComponent(Root);

	SiteCollision->SetupAttachment(Root);

	//We often trace on a pixel perfect validation traces - reduce this bounds slighty here to account for perfectly flush placements
	SiteCollision->SetBoxExtent(GridSettings->GetCellBounds() * 0.9f);
}

void AConstructionSite::SetConstructionRequest(TSharedPtr<FConstructionRequest> InSourceRequest)
{
	SourceRequest = InSourceRequest;

	TWeakObjectPtr<AConstructionSite> WeakThis = TWeakObjectPtr<AConstructionSite>(this);
	SourceRequest->OnRequestCompleted.AddLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->OnComplete();
		}
	});

	InitialiseDecals();
}

void AConstructionSite::OnComplete()
{
	//Replace this with something nice at a later date
	Destroy();
}

void AConstructionSite::InitialiseDecals()
{
	AssetLoader->RequestLoad<UMaterialInterface>(DecalMaterialClass, [WeakThis = TWeakObjectPtr<AConstructionSite>(this), this](UMaterialInterface& LoadedMaterial)
	{
		if (WeakThis.IsValid())
		{
			for (const FFoundationPoint& Point : SourceRequest->Source.BuildingFoundation.Points)
			{
				UDecalComponent* NewDecal = NewObject<UDecalComponent>(this);
				NewDecal->RegisterComponent();

				NewDecal->SetMaterial(0, &LoadedMaterial);

				const FVector ImpactNormal = Point.HitRes.ImpactNormal.GetSafeNormal();
				const FRotator DecalRot = ImpactNormal.Rotation();

				NewDecal->SetRelativeRotation(DecalRot);
				NewDecal->SetWorldLocation(Point.HitRes.Location);
				NewDecal->DecalSize = FVector(GridSettings->GridCellSize, GridSettings->GridCellSize, GridSettings->GridCellSize);

				SpawnedDecals.Add(NewDecal);
			}
		}
	});
}
