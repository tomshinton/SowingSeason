#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Buildings/Public/FootprintProviderInterface.h"

#include <Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <Runtime/Selection/Public/SelectionInterface.h>

#include "Building.generated.h"

class USceneComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> SelectionBoxClass;

	UPROPERTY(EditDefaultsOnly, Category = UI, meta = (MakeEditWidget = true))
	FVector SelectionBoxLocation;

	//IFootprintProviderInterface
	const UPrimitiveComponent& GetFootprintReference() const override;
	//~IFootprintProviderInterface

	//ISelectionInterface
	virtual void OnSelected() override;
	virtual void OnUnselected() override;
	virtual TSubclassOf<UUserWidget> GetSelectionBoxClass() const override;
	virtual FVector GetSelectionBoxLocation() const override;

#if !UE_BUILD_SHIPPING
	virtual FString GetDebugName() const override;
#endif //!UE_BUILD_SHIPPING
	//~ISelectionInterface
};