// Hydrogen Project - Tom Shinton 2020

#pragma once

#include <Runtime/Engine/Classes/Engine/DeveloperSettings.h>

#include "MovementSettings.generated.h"

UCLASS(config = Game, defaultconfig, MinimalAPI)
class UMovementSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UMovementSettings()
		: EdgePadding_Major(20.f)
		, EdgePadding_Bottom(10.f)
		, MaxEdgeMoveStrength(1.f)
		, MiddleMouseButtonMoveStrength(1.f)
		, MaxArmLength(10000.f)
		, MinArmLength(1000.f)
		, ArmZoomRate(1000.f)
		, CameraZoomSpeed(2.f)
		, MoveSpeed(1.f)
		, RotateSpeed(1.f)
	{};

	UPROPERTY(config, EditDefaultsOnly, Category = "Edge Movement")
	float EdgePadding_Major;

	UPROPERTY(config, EditDefaultsOnly, Category = "Edge Movement")
	float EdgePadding_Bottom;

	UPROPERTY(config, EditDefaultsOnly, Category = "Edge Movement")
	float MaxEdgeMoveStrength;

	UPROPERTY(config, EditDefaultsOnly, Category = "Edge Movement")
	float MiddleMouseButtonMoveStrength;

	UPROPERTY(config, EditDefaultsOnly, Category = "Camera")
	float MaxArmLength;

	UPROPERTY(config, EditDefaultsOnly, Category = "Camera")
	float MinArmLength;

	UPROPERTY(config, EditDefaultsOnly, Category = "Camera")
	float ArmZoomRate;

	UPROPERTY(config, EditDefaultsOnly, Category = "Camera")
	float CameraZoomSpeed;

	UPROPERTY(config, EditDefaultsOnly, Category = "Speed")
	float MoveSpeed;

	UPROPERTY(config, EditDefaultsOnly, Category = "Speed")
	float RotateSpeed;
};