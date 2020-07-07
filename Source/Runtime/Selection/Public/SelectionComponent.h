#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>

#include "SelectionComponent.generated.h"

class APlayerController;
class ISelectionInterface;

UCLASS(MinimalAPI)
class USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USelectionComponent();

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> SelectionChannel;

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

	TWeakInterfacePtr<ISelectionInterface> CurrentHoverInterface;
	TWeakInterfacePtr<ISelectionInterface> CurrentSelectionInterface;
};