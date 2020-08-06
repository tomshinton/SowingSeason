// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Tasks/BTTask_RequestGarrison.h"

#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/Buildings/Public/Garrison/GarrisonInterface.h>

DEFINE_LOG_CATEGORY_STATIC(TaskFindGarrisonPointLog, Log, All)

UBTTask_RequestGarrison::UBTTask_RequestGarrison()
	: TargetGarrisonActor()
	, BlackboardComp(nullptr)
{
	NodeName = "Request Garrison";

	TargetGarrisonActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RequestGarrison, TargetGarrisonActor), UObject::StaticClass());
}

EBTNodeResult::Type UBTTask_RequestGarrison::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BlackboardComp = OwnerComp.GetBlackboardComponent();

	check(BlackboardComp != nullptr);

	if (UObject* Target = BlackboardComp->GetValueAsObject(TargetGarrisonActor.SelectedKeyName))
	{
		if (AActor* TargetAsActor = Cast<AActor>(Target))
		{
			if (IGarrisonInterface* GarrisonInterface = TargetAsActor->GetInterface<IGarrisonInterface>())
			{
				return GarrisonInterface->RequestGarrison(*OwnerComp.GetOwner()) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Failed;
}

FString UBTTask_RequestGarrison::GetStaticDescription() const
{
	Super::GetStaticDescription();

	return FString::Printf(TEXT("Request garrison in %s"), *TargetGarrisonActor.SelectedKeyName.ToString());
}