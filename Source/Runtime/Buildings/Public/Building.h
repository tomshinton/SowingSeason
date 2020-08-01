#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Buildings/Public/FootprintProviderInterface.h"

#include <Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <Runtime/Selection/Public/SelectionInterface.h>

#include "Building.generated.h"

class USceneComponent;
class UInventoryComponent;

UCLASS(MinimalAPI)
class ABuilding : public AActor
	, public IFootprintProvider
	, public ISelectionInterface

{
	GENERATED_BODY()

public:

	ABuilding(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BuildingRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> SelectionBoxClass;

	UPROPERTY(EditDefaultsOnly, Category = UI, meta = (MakeEditWidget = true))
	FVector SelectionBoxLocation;

	UPROPERTY(EditDefaultsOnly)
	FText BuildingName;

	//IFootprintProviderInterface
	const UPrimitiveComponent& GetFootprintReference() const override;
	//~IFootprintProviderInterface

	//ISelectionInterface
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual TSubclassOf<UUserWidget> GetSelectionBoxClass() const override;
	virtual FVector GetSelectionBoxLocation() const override;
	virtual AActor& GetActor() override;
	virtual FText GetSelectionName() const override;
	//~ISelectionInterface
};