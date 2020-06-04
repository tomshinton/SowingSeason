// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

#include "SowingSeasonPlayerPawn.generated.h"

class UBuildComponent;
class UCameraComponent;
class URTSMovementComponent;
class USceneComponent;
class USpringArmComponent;

UCLASS()
class ASowingSeasonPlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	ASowingSeasonPlayerPawn(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	USceneComponent* PawnRootComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	URTSMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	UBuildComponent* BuildComponent;
};
