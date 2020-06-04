#pragma once

#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include "Runtime/Build/Public/Ghost/GhostInterface.h"

#include "BuildingGhost.generated.h"

UCLASS()
class ABuildingGhost : public AActor
	, public IGhostInterface
{
	GENERATED_BODY()

public:

	ABuildingGhost(const FObjectInitializer& ObjectInitializer);

	//IGhostInterface
	void SetGhostInfo(const UBuildingData& InSourceData) override;
	void DestroyGhost() override;
	//~IGhostInterface

private:

	UPROPERTY(Transient)
	const UBuildingData* SourceBuildingData;
};