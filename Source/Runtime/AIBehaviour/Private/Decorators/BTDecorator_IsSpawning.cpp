// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Decorators/BTDecorator_IsSpawning.h"

UBTDecorator_IsSpawning::UBTDecorator_IsSpawning()
{

}

FString UBTDecorator_IsSpawning::GetStaticDescription() const
{
	return TEXT("Is this pawn current in the process of being intialised?");
}

bool UBTDecorator_IsSpawning::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (AController* OwnerAsController = Cast<AController>(OwnerComp.GetOwner()))
	{
		if (APawn* OwningPawn = OwnerAsController->GetPawn())
		{
			return OwningPawn->HasActorBegunPlay();
		}
	}
	return false;
}
