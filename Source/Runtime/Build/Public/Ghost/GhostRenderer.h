#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Build/Public/Ghost/GhostRendererInterface.h"

#include "GhostRenderer.generated.h"

UCLASS()
class AGhostRenderer : public AActor
	, public IGhostRendererInterface
{
	GENERATED_BODY()

public:

	AGhostRenderer(const FObjectInitializer& ObjectInitializer);

	//IGhostInterface
	virtual void SetGhostInfo(const UBuildingData& InSourceData) override;
	//~IGhostInterface

private:

	UPROPERTY(Transient)
	const UBuildingData* SourceBuildingData;
};