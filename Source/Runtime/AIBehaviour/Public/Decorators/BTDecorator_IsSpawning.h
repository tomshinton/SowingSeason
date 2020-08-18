// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BTDecorator.h>

#include "BTDecorator_IsSpawning.generated.h"

UCLASS()
class UBTDecorator_IsSpawning : public UBTDecorator
{
	GENERATED_BODY()

public:

	UBTDecorator_IsSpawning();

private:

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	FString GetStaticDescription() const override;
};
