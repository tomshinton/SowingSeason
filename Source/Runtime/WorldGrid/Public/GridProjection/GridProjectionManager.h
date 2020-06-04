// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/CoreUObject/Public/UObject/Object.h>
 
#include "Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h"

#include "GridProjectionManager.generated.h"

class APlayerController;
class UWorldGridSettings;

UCLASS(MinimalAPI)
class UGridProjectionManager : public UObject
	, public IGridProjectionInterface
{
	GENERATED_BODY()

public:

	UGridProjectionManager();

	WORLDGRID_API void Init(const UWorld& InWorld);

private:

	UPROPERTY()
	const UWorld* CachedWorld;

	UPROPERTY()
	APlayerController* LocalPlayerController;

	UPROPERTY()
	const UWorldGridSettings* WorldGridSettings;

	//IGridLocationManagerInterface
	const FVector* GetPositionUnderMouse() const override;
	const FVector* GetRoundedPositionUnderMouse() const override;
	FOnRoundedPositionChanged& GetOnRoundedPositionChanged() override;
	FOnPositionChanged& GetOnPositionChanged() override;
	//~IGridLocationManagerInterface

	void QueryScene();
	void ProcessHitResult(const FHitResult& InHitResult);
	
	void ProcessNewPosition(const FVector& InNewPosition);
	void ProcessNewRoundedPosition(const FVector& InNewRoundedPosition);

	FVector RoundedPosition;
	FVector Position;

	FTimerHandle UpdatePositionHandle;

	FOnRoundedPositionChanged OnRoundedPositionChanged;
	FOnPositionChanged OnPositionChanged;

	FCollisionQueryParams TraceParams;
};