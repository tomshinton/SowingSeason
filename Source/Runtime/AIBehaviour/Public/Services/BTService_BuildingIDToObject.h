// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BTService.h>

#include "BTService_BuildingIDToObject.generated.h"

class IIdentityInterface;

UCLASS()
class UBTService_BuildingIDToObject : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_BuildingIDToObject();

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector BuildIDKey;

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector TargetKey;

private:

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FString GetStaticDescription() const override;
};
