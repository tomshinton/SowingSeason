#include "Runtime/Buildings/Public/Building.h"

namespace BuildingPrivate
{
	const FName BuildingRootName = TEXT("BuildingRoot");
	const FName BuildingMeshName = TEXT("BuildingMesh");

	const FName BuildingCollisionProfileName = TEXT("Building");
}

ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BuildingRoot(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, BuildingPrivate::BuildingRootName))
	, BuildingMesh(ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, BuildingPrivate::BuildingMeshName))
{
	RootComponent = BuildingRoot;

	BuildingMesh->SetupAttachment(BuildingRoot);
	BuildingMesh->SetCollisionProfileName(BuildingPrivate::BuildingCollisionProfileName);
}

const UPrimitiveComponent& ABuilding::GetFootprintReference() const
{
	BuildingMesh->UpdateBounds();
	return *BuildingMesh;
}

void ABuilding::OnSelected()
{

}

void ABuilding::OnUnselected()
{

}

FString ABuilding::GetDebugName() const
{
	return GetName();
}

