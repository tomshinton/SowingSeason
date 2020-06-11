#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Buildings/Public/FootprintProviderInterface.h"

#include <Runtime/Engine/Classes/Components/StaticMeshComponent.h>

#include "Building.generated.h"

class USceneComponent;

UCLASS()
class ABuilding : public AActor
	, public IFootprintProvider

{
	GENERATED_BODY()

public:

	ABuilding(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	USceneComponent* BuildingRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BuildingMesh;

	//IFootprintProviderInterface
	const UPrimitiveComponent& GetFootprintReference() const override;
	//~IFootprintProviderInterface
};