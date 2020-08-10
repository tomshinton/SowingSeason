// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Tasks/BTTask_FindGarrisonPoint.h"

#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/Buildings/Public/Garrison/GarrisonInterface.h>

DEFINE_LOG_CATEGORY_STATIC(TaskFindGarrisonPointLog, Log, All)

UBTTask_FindGarrisonPoint::UBTTask_FindGarrisonPoint()
	: FindType(EFindType::FromObjectKey)
	, TargetGarrisonActor()
	, KeyToSet()
	, BlackboardComp(nullptr)
{
	NodeName = "Find Garrison Point";

	TargetGarrisonActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindGarrisonPoint, TargetGarrisonActor), UObject::StaticClass());
	KeyToSet.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindGarrisonPoint, KeyToSet));
}

FString UBTTask_FindGarrisonPoint::GetStaticDescription() const
{
	Super::GetStaticDescription();

	switch (FindType)
	{
	case EFindType::FromObjectKey:
		return FString::Printf(TEXT("Sourcing Garrison Point from %s"), *TargetGarrisonActor.SelectedKeyName.ToString());
		break;
	case EFindType::Nearest:
		return FString::Printf(TEXT("Sourcing Garrison Point from Nearest Building"));
		break;
	}

	return "";
}

EBTNodeResult::Type UBTTask_FindGarrisonPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BlackboardComp = OwnerComp.GetBlackboardComponent();

	check(BlackboardComp != nullptr);

	switch (FindType)
	{
	case EFindType::FromObjectKey:
		return GetLocationFromObject();
		break;
	case EFindType::Nearest:
		return GetLocationFromNearest();
		break;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_FindGarrisonPoint::GetLocationFromObject()
{
	if (UObject* Target = BlackboardComp->GetValueAsObject(TargetGarrisonActor.SelectedKeyName))
	{
		if (AActor* TargetAsActor = Cast<AActor>(Target))
		{
			if (IGarrisonInterface* GarrisonOnTarget = TargetAsActor->GetInterface<IGarrisonInterface>())
			{
				if (GarrisonOnTarget->HasDoors())
				{
					SetTargetLocation(GarrisonOnTarget->GetRandomDoor().GetComponentLocation());
					return EBTNodeResult::Succeeded;
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_FindGarrisonPoint::GetLocationFromNearest()
{
	return EBTNodeResult::Failed;
}

void UBTTask_FindGarrisonPoint::SetTargetLocation(const FVector& InLocation)
{
	BlackboardComp->SetValueAsVector(KeyToSet.SelectedKeyName, InLocation);
}
