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
	, SelectionBoxClass(nullptr)
	, SelectionBoxLocation(FVector::ZeroVector)
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
	//Add outline, play sound, etc etc
}

void ABuilding::OnUnselected()
{

}

FText ABuilding::GetSelectionName() const
{
	return BuildingName;
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

