// Sowing Season - Tom Shinton 2020

#include "Runtime/AICore/Public/Core/VillagerPawn.h"

#include "Runtime/AICore/Public/Core/VillagerController.h"
#include "Runtime/AICore/Public/Identity/IdentityComponent.h"

namespace VillagerPawnPrivate
{
	const FName IdentityComponentName = TEXT("IdentityComponent");
}

AVillagerPawn::AVillagerPawn(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
	, IdentityComponent(ObjectInitialiser.CreateDefaultSubobject<UIdentityComponent>(this, VillagerPawnPrivate::IdentityComponentName))
{
	AIControllerClass = AVillagerController::StaticClass();
}
