// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/Pawn.h>

#include "VillagerPawn.generated.h"

class UIdentityComponent;

UCLASS(abstract, Blueprintable, MinimalAPI)
class AVillagerPawn : public APawn
{
	GENERATED_BODY()

public:

	AVillagerPawn(const FObjectInitializer& ObjectInitialiser);

	UPROPERTY(VisibleAnywhere)
	UIdentityComponent* IdentityComponent;
};
