// Sowing Season - Tom Shinton 2020

#pragma once

#include "Runtime/Core/Public/CoreMinimal.h"

#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>

#include "BTTask_FindGarrisonPoint.generated.h"

class UBlackboardComponent;

UENUM()
enum class EFindType : uint8 
{
	Nearest, 
	FromObjectKey
};

UCLASS()
class UBTTask_FindGarrisonPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_FindGarrisonPoint();

	UPROPERTY(Category = Node, EditAnywhere)
	EFindType FindType;

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector TargetGarrisonActor;

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector KeyToSet;

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FString GetStaticDescription() const override;

	EBTNodeResult::Type GetLocationFromObject();
	EBTNodeResult::Type GetLocationFromNearest();

	void SetTargetLocation(const FVector& InLocation);

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;
};
