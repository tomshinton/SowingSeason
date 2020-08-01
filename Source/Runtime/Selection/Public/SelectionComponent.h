#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>

#include "Runtime/Selection/Public/SelectionInterface.h"

#include "SelectionComponent.generated.h"

class APlayerController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelect, const TWeakInterfacePtr<ISelectionInterface>& /*New interface*/);
DECLARE_MULTICAST_DELEGATE(FOnDeselect);

UCLASS(MinimalAPI)
class USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USelectionComponent();

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> SelectionChannel;

	FOnSelect& GetOnSelectDelegate() { return OnSelect; }
	FOnDeselect& GetOnDeselectDelegate() { return OnDeselect; }

	TWeakInterfacePtr<ISelectionInterface> GetCurrentSelection() const { return CurrentSelectionInterface; }

private:

	void BeginPlay() override;
	void SetupComponentInputBindings(UInputComponent& PlayerInputComponent) override;
	void QueryScene();
	void OnTraceComplete(const FTraceDatum& InTraceData);

	void TrySelect();
	void Deselect();

	UPROPERTY()
	UWorld* CachedWorld;

	UPROPERTY()
	APlayerController* OwningPlayerController;

	FTimerHandle TraceTimerHandle;

	FTraceHandle TraceHandle;
	FTraceDelegate TraceDelegate;

	FOnSelect OnSelect;
	FOnDeselect OnDeselect;

	TWeakInterfacePtr<ISelectionInterface> CurrentHoverInterface;
	TWeakInterfacePtr<ISelectionInterface> CurrentSelectionInterface;
};