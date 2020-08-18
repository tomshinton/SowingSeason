// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Services/BTService_BuildingIDToObject.h"

#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/Buildings/Public/Building.h>
#include <Runtime/Buildings/Public/BuildingInterface.h>
#include <Runtime/Engine/Public/EngineUtils.h>

DEFINE_LOG_CATEGORY_STATIC(FindResidenceServiceLog, Log, Log)

UBTService_BuildingIDToObject::UBTService_BuildingIDToObject()
{
	NodeName = "Convert BuildingID to Object";

	BuildIDKey.AddGuidFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_BuildingIDToObject, BuildIDKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_BuildingIDToObject, TargetKey), UObject::StaticClass());
}

FString UBTService_BuildingIDToObject::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set object key %s from building guid %s"), *TargetKey.SelectedKeyName.ToString(), *BuildIDKey.SelectedKeyName.ToString());
}

void UBTService_BuildingIDToObject::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	const FGuid SourceGuid = Blackboard->GetValueAsGuid(BuildIDKey.SelectedKeyName);

	for (TActorIterator<ABuilding> Itr(OwnerComp.GetWorld()); Itr; ++Itr)
	{
		if (AActor* Actor = *Itr)
		{
			if (IBuildingInterface* BuildingInterface = Actor->GetInterface<IBuildingInterface>())
			{
				if (BuildingInterface->GetBuildingID() == SourceGuid)
				{
					Blackboard->SetValueAsObject(TargetKey.SelectedKeyName, Actor);
					return;
				}
			}
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
