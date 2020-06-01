// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "Runtime/Movement/Public/EdgeBandDefinition.h"

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "RTSMovementComponent.generated.h"

class APlayerController;
class USpringArmComponent;
class UMovementSettings;

//////////////////////////////////////////////////////////////////////////
// RTS Movement class, controls how the pawn moves, zooms, and scrolls around the viewport
//////////////////////////////////////////////////////////////////////////

UCLASS( MinimalAPI, ClassGroup=(Movement), meta=(BlueprintSpawnableComponent) )
class URTSMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URTSMovementComponent();

	UPROPERTY(Transient)
	const UMovementSettings* MovementSettings;

	UPROPERTY()
	USpringArmComponent* CameraArm;

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupComponentInputBindings(UInputComponent& PlayerInputComponent) override;

	void InitialiseCameraView();

	void MoveForwards(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);

	void StartMiddleMouseMove();
	void EndMiddleMouseMove();

	void UpdateMouseLocation();

	const FEdgeBands* GetEdgeBands() const;

private:

	void TryEdgeMove(const float InDeltaTime);
	void RotateCamera(const float InDeltaTime);
	void BlendCameraZoom(const float InDeltaTime);

	void MiddleMouseButtonMove();

	void SetViewportSize(const FVector2D InNewViewportSize);

	void ZoomIn();
	void ZoomOut();

	UPROPERTY()
	AActor* OwningPawn;

	UPROPERTY()
	APlayerController* OwningController;

	float TargetYaw;
	float TargetArmLength;

	FVector2D CurrMousePos;
	FVector2D StoredMousePos;
	FVector2D ViewportSize;
	bool UsingMiddleMouseMovement;
	FTimerHandle MiddleMouseMoveTimer;

	FEdgeBands EdgeBands;
};
