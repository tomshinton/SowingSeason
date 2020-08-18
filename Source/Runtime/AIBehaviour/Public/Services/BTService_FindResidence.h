// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BTService.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>

#include "BTService_FindResidence.generated.h"

class IIdentityInterface;

struct FBTService_FindResidenceMemory
{
public:

	TWeakInterfacePtr<IIdentityInterface> OwnerIdentity;
};

UCLASS()
class UBTService_FindResidence : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_FindResidence();

	UPROPERTY(Category = Node, EditAnywhere)
	struct FBlackboardKeySelector ResidenceKey;

private:

	void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FString GetStaticDescription() const override;
	void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	uint16 GetInstanceMemorySize() const override;
};
