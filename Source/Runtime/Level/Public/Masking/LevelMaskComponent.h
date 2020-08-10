// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "LevelMaskComponent.generated.h"

class ALandscape;
class UTexture2D;
class UWorldGridSettings;

UCLASS()
class ULevelMaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	ULevelMaskComponent();

	UFUNCTION(BlueprintPure, Category = "Mask")
	UTexture2D* GetMask();

	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	bool ShouldGenerateMask;

private:

	void GenerateMask();
	void SanitizeBounds();

	UPROPERTY()
	UTexture2D* Mask;

	UPROPERTY()
	const UWorldGridSettings* WorldGridSettings;

	FVector2D SourceBounds;
};
