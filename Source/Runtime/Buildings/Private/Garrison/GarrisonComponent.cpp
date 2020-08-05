#include "Runtime/Buildings/Public/Garrison/GarrisonComponent.h"
#include "Runtime/Buildings/Public/Architecture/DoorComponent.h"

#include <Runtime/Engine/Classes/GameFramework/Character.h>

DEFINE_LOG_CATEGORY_STATIC(GarrisonComponentLog, Log, All)

UGarrisonComponent::UGarrisonComponent(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
	, Doors()
{

}

void UGarrisonComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheDoorsOnOwner();
}

void UGarrisonComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UDoorComponent* Door : Doors)
	{
		Door->GetOnGarrisonRequested().RemoveAll(this);
		Door->GetOnGarrisonUnrequested().RemoveAll(this);
	}
}

void UGarrisonComponent::CacheDoorsOnOwner()
{
	if (AActor* OwningActor = GetOwner())
	{
		TArray<UActorComponent*> FoundComponents = OwningActor->GetComponentsByClass(UDoorComponent::StaticClass());

		if (FoundComponents.Num() > 0)
		{
			Doors.Reserve(FoundComponents.Num());

			for (UActorComponent* Component : FoundComponents)
			{
				if (UDoorComponent* Door = CastChecked<UDoorComponent>(Component))
				{
					Doors.Add(Door);

					Door->GetOnGarrisonRequested().AddUObject(this, &UGarrisonComponent::OnGarrisonRequested);
					Door->GetOnGarrisonUnrequested().AddUObject(this, &UGarrisonComponent::OnUngarrisonRequested);
				}
			}
		}
		else
		{
			UE_LOG(GarrisonComponentLog, Error, TEXT("Garrison component found on %s with no UDoorComponents"), *OwningActor->GetName());
		}
	}
}

void UGarrisonComponent::OnGarrisonRequested(ACharacter* InRequestingChar)
{
	UE_LOG(GarrisonComponentLog, Log, TEXT("%s requesting garrison in %s"), *InRequestingChar->GetName(), *GetOwner()->GetName());

}

void UGarrisonComponent::OnUngarrisonRequested(ACharacter* InRequestingChar)
{
	UE_LOG(GarrisonComponentLog, Log, TEXT("%s requesting ungarrison in %s"), *InRequestingChar->GetName(), *GetOwner()->GetName());

}
