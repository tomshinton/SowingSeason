// Sowing Season - Tom Shinton 2020

#include "Runtime/Buildings/Public/Building.h"

#include <Runtime/Inventory/Public/InventoryComponent.h>

namespace BuildingPrivate
{
	const FName BuildingRootName = TEXT("BuildingRoot");
	const FName BuildingMeshName = TEXT("BuildingMesh");
	const FName InventoryComponentName = TEXT("InventoryComponent");

	const FName BuildingCollisionProfileName = TEXT("Building");
}

ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BuildingRoot(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, BuildingPrivate::BuildingRootName))
	, BuildingMesh(ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, BuildingPrivate::BuildingMeshName))
	, InventoryComponent(ObjectInitializer.CreateDefaultSubobject<UInventoryComponent>(this, BuildingPrivate::InventoryComponentName))
	, SelectionBoxClass(nullptr)
	, SelectionBoxLocation(FVector::ZeroVector)
	, BuildingID(FGuid::NewGuid())
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

FText ABuilding::GetSelectionName() const
{
	return BuildingName;
}

FGuid ABuilding::GetBuildingID() const
{
	return BuildingID;
}

FVector ABuilding::GetSelectionBoxLocation() const
{
	return GetActorLocation() + SelectionBoxLocation;
}

AActor& ABuilding::GetActor()
{
	return *this;
}

TSubclassOf<UUserWidget> ABuilding::GetSelectionBoxClass() const
{
	return SelectionBoxClass;
}

