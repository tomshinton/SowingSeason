// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Public/SowingSeasonPlayerPawn.h"

#include <Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Runtime/Build/Public/BuildComponent.h>
#include <Runtime/Movement/Public/RTSMovementComponent.h>

namespace PlayerPawnPrivate
{
	const FName PawnRootComponentName = TEXT("PawnRoot");
	const FName SpringArmComponentName = TEXT("PlayerCameraArm");
	const FName CameraComponentName = TEXT("PlayerCamera");
	const FName MovementComponentName = TEXT("MovementComponent");
	const FName BuildComponentName = TEXT("BuildComponent");
}

ASowingSeasonPlayerPawn::ASowingSeasonPlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PawnRootComponent(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, PlayerPawnPrivate::PawnRootComponentName))
	, SpringArm(ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, PlayerPawnPrivate::SpringArmComponentName))
	, CameraComponent(ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, PlayerPawnPrivate::CameraComponentName))
	, MovementComponent(ObjectInitializer.CreateDefaultSubobject<URTSMovementComponent>(this, PlayerPawnPrivate::MovementComponentName))
	, BuildComponent(ObjectInitializer.CreateDefaultSubobject<UBuildComponent>(this, PlayerPawnPrivate::BuildComponentName))
{
	RootComponent = PawnRootComponent;
	SpringArm->SetupAttachment(PawnRootComponent);
	CameraComponent->SetupAttachment(SpringArm);
}

