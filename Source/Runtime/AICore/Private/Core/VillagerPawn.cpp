// Sowing Season - Tom Shinton 2020

#include "Runtime/AICore/Public/Core/VillagerPawn.h"

#include "Runtime/AICore/Public/Core/VillagerController.h"

AVillagerPawn::AVillagerPawn(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
{
	AIControllerClass = AVillagerController::StaticClass();
}
