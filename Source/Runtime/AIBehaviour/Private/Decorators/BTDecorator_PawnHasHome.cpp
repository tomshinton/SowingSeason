// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Decorators/BTDecorator_PawnHasHome.h"

#include <Runtime/AIIdentity/Public/IdentityInterface.h>

UBTDecorator_PawnHasHome::UBTDecorator_PawnHasHome()
{

}

void UBTDecorator_PawnHasHome::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	FBTDecorator_PawnHasHomeMemory* Memory = reinterpret_cast<FBTDecorator_PawnHasHomeMemory*>(NodeMemory);

	if(AController* OwnerAsController = Cast<AController>(OwnerComp.GetOwner()))
	{
		if (APawn* OwningPawn = OwnerAsController->GetPawn())
		{
			if (IIdentityInterface* IdentityInterface = OwningPawn->GetInterface<IIdentityInterface>())
			{
				Memory->OwnerIdentity = *IdentityInterface;
			}
		}
	}
}

FString UBTDecorator_PawnHasHome::GetStaticDescription() const
{
	return TEXT("Return true if pawn has a valid residence");
}

uint16 UBTDecorator_PawnHasHome::GetInstanceMemorySize() const
{
	return sizeof(FBTDecorator_PawnHasHomeMemory);
}

bool UBTDecorator_PawnHasHome::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	if (FBTDecorator_PawnHasHomeMemory* Memory = reinterpret_cast<FBTDecorator_PawnHasHomeMemory*>(NodeMemory))
	{
		if (Memory->OwnerIdentity.IsValid())
		{
			return Memory->OwnerIdentity->GetResidenceID().IsValid();
		}
	}

	return false;
}
