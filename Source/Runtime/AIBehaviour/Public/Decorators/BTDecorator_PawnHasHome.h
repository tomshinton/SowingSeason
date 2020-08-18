// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BTDecorator.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>

#include "BTDecorator_PawnHasHome.generated.h"

class IIdentityInterface;

struct FBTDecorator_PawnHasHomeMemory
{
	TWeakInterfacePtr<IIdentityInterface> OwnerIdentity;
};

UCLASS()
class UBTDecorator_PawnHasHome : public UBTDecorator
{
	GENERATED_BODY()

public:

	UBTDecorator_PawnHasHome();

private:

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	FString GetStaticDescription() const override;
	uint16 GetInstanceMemorySize() const override;
	void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
};
