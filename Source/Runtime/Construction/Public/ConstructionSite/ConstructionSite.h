#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/Engine/Classes/GameFramework/Actor.h>

#include <Runtime/Core/Public/Templates/UniquePtr.h>
#include <Runtime/CoreUObject/Public/Serialization/AsyncLoader.h>
#include <Runtime/Engine/Classes/Components/DecalComponent.h>
#include <Runtime/Engine/Classes/Materials/MaterialInterface.h>

#include "ConstructionSite.generated.h"

class UWorldGridSettings;
class UBoxComponent;
class USceneComponent;

struct FConstructionRequest;

UCLASS()
class AConstructionSite : public AActor
{
	GENERATED_BODY()

public:

	AConstructionSite(const FObjectInitializer& ObjectInitializer);

	void SetConstructionRequest(TSharedPtr<FConstructionRequest> InSourceRequest);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* SiteCollision;

	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath DecalMaterialClass;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAdvanced(const float InNewAmount, const EConstructionState InNewState);

private:

	void OnComplete();
	

	void InitialiseDecals();

	TSharedPtr<FConstructionRequest> SourceRequest;

	UPROPERTY()
	TArray<UDecalComponent*> SpawnedDecals;

	UPROPERTY()
	const UWorldGridSettings* GridSettings;

	TUniquePtr<FAsyncLoader> AssetLoader;
};