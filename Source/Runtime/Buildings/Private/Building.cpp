#include "Runtime/Buildings/Public/Building.h"

namespace BuildingPrivate
{
	const FName BuildingRootName = TEXT("BuildingRoot");
}

ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BuildingRoot(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, BuildingPrivate::BuildingRootName))
{
	RootComponent = BuildingRoot;
}

