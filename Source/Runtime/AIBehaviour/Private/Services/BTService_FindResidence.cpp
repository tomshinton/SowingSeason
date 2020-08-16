// Sowing Season - Tom Shinton 2020

#include "Runtime/AIBehaviour/Public/Services/BTService_FindResidence.h"

#include <Runtime/AIIdentity/Public/IdentityInterface.h>
#include <Runtime/Buildings/Public/Residence/ResidenceInterface.h>
#include <Runtime/Engine/Public/EngineUtils.h>

DEFINE_LOG_CATEGORY_STATIC(FindResidenceServiceLog, Log, Log)

UBTService_FindResidence::UBTService_FindResidence()
{
	NodeName = "Find Viable Residence";

	ResidenceActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindResidence, ResidenceActorKey), UObject::StaticClass());
}

FString UBTService_FindResidence::GetStaticDescription() const
{
	Super::GetStaticDescription();

	return TEXT("Find a valid place for this Pawn to claim as their's family home");
}

void UBTService_FindResidence::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	FBTService_FindResidenceMemory* Memory = reinterpret_cast<FBTService_FindResidenceMemory*>(NodeMemory);

	if (Memory->OwnerIdentity.IsValid())
	{
		if (Memory->OwnerIdentity->GetResidenceID().IsValid())
		{
			Values.Add(FString::Printf(TEXT("Pawn residence: %s"), *Memory->OwnerIdentity->GetResidenceID().ToString()));
		}
		else
		{
			Values.Add(TEXT("Pawn has no home"));
		}
	}

	Values.Add(TEXT("ERROR: Pawn has no identity"));
}

uint16 UBTService_FindResidence::GetInstanceMemorySize() const
{
	return sizeof(FBTService_FindResidenceMemory);
}

void UBTService_FindResidence::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTService_FindResidenceMemory* Memory = reinterpret_cast<FBTService_FindResidenceMemory*>(NodeMemory);

	if (!Memory->OwnerIdentity.IsValid())
	{
		if (AController* ControllerAsOwner = Cast<AController>(OwnerComp.GetOwner()))
		{
			if (IIdentityInterface* OwningIdentityInterface = ControllerAsOwner->GetPawn()->GetInterface<IIdentityInterface>())
			{
				Memory->OwnerIdentity = *OwningIdentityInterface;
			}
		}
		else
		{
			UE_LOG(FindResidenceServiceLog, Error, TEXT("%s has no IdentityInterface - cannot find residence for Pawn with no Identity Component"), *OwnerComp.GetOwner()->GetName());
			return;
		}
	}

	if (Memory->OwnerIdentity.IsValid() && !Memory->OwnerIdentity->GetResidenceID().IsValid())
	{
		for (TActorIterator<AActor> Itr(OwnerComp.GetWorld()); Itr; ++Itr)
		{
			if (AActor* Actor = *Itr)
			{
				if (IResidenceInterface* ResidenceInterface = Actor->GetInterface<IResidenceInterface>())
				{
					const FGuid FamilyID = Memory->OwnerIdentity->GetFamilyID();
					const FGuid VillagerID = Memory->OwnerIdentity->GetVillagerID();

					const FGuid FoundResidence = ResidenceInterface->GetResidenceID();

					if (ResidenceInterface->CanOccupy(FamilyID))
					{
						ResidenceInterface->Occupy(VillagerID, FamilyID);
						Memory->OwnerIdentity->SetResidence(FoundResidence);

						UE_LOG(FindResidenceServiceLog, Log, TEXT("Pawn %s now lives at %s"), *VillagerID.ToString(), *FoundResidence.ToString());
					}
				}
			}
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
