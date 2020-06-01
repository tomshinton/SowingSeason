#include "SowingSeason/Public/SowingSeasonPlayerPawn.h"

#include <Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Runtime/Movement/Public/RTSMovementComponent.h>

namespace PlayerPawnPrivate
{
	const FName MovementComponentName = TEXT("MovementComponent");
	const FName SpringArmComponentName = TEXT("PlayerCameraArm");
	const FName CameraComponentName = TEXT("PlayerCamera");
	const FName PawnRootComponentName = TEXT("PawnRoot");
}

ASowingSeasonPlayerPawn::ASowingSeasonPlayerPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PawnRootComponent(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, PlayerPawnPrivate::PawnRootComponentName))
	, MovementComponent(ObjectInitializer.CreateDefaultSubobject<URTSMovementComponent>(this, PlayerPawnPrivate::MovementComponentName))
	, SpringArm(ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, PlayerPawnPrivate::SpringArmComponentName))
	, CameraComponent(ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, PlayerPawnPrivate::CameraComponentName))
{
	RootComponent = PawnRootComponent;
	SpringArm->SetupAttachment(PawnRootComponent);
	CameraComponent->SetupAttachment(SpringArm);
}

