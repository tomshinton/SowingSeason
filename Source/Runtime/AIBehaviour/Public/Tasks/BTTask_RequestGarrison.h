// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"

#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>

#include "BTTask_RequestGarrison.generated.h"

UCLASS()
class UBTTask_RequestGarrison : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_RequestGarrison();

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector TargetGarrisonActor;

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FString GetStaticDescription() const override;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;
};
