#pragma once

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Runtime/Buildings/Public/Garrison/GarrisonInterface.h"

#include <Runtime/Engine/Classes/GameFramework/Character.h>

#include "GarrisonComponent.generated.h"

class UDoorComponent;

UCLASS(meta=(BlueprintSpawnableComponent))
class UGarrisonComponent : public UActorComponent
	, public IGarrisonInterface
{
	GENERATED_BODY()

public:

	UGarrisonComponent(const FObjectInitializer& ObjectInitialiser);

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	void CacheDoorsOnOwner();
	
	UFUNCTION()
	void OnGarrisonRequested(ACharacter* InRequestingChar);

	UFUNCTION()
	void OnUngarrisonRequested(ACharacter* InRequestingChar);

	UPROPERTY()
	TArray<UDoorComponent*> Doors;
};
