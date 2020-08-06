#include "Runtime/Buildings/Public/Garrison/GarrisonComponent.h"
#include "Runtime/Buildings/Public/Architecture/DoorComponent.h"

#include <Runtime/Engine/Classes/GameFramework/Controller.h>

DEFINE_LOG_CATEGORY_STATIC(GarrisonComponentLog, Log, All)

UGarrisonComponent::UGarrisonComponent(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
	, Doors()
	, GarrisonedControllers()
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

bool UGarrisonComponent::RequestGarrison(AActor& InRequestingActor)
{
	if (AController* RequestAsController = Cast<AController>(&InRequestingActor))
	{
		if (APawn* ControlledPawn = RequestAsController->GetPawn())
		{
			ControlledPawn->SetActorHiddenInGame(true);
			ControlledPawn->SetActorLocation(GetOwner()->GetActorLocation());
		}

		GarrisonedControllers.AddUnique(RequestAsController);
		return true;
	}

	return false;
}

bool UGarrisonComponent::RequestUngarrison(AActor& InRequestingActor)
{
	if (AController* RequestAsController = Cast<AController>(&InRequestingActor))
	{
		if (APawn* ControlledPawn = RequestAsController->GetPawn())
		{
			InRequestingActor.SetActorHiddenInGame(false);
			InRequestingActor.SetActorLocation(GetRandomDoor().GetComponentLocation());
		}

		GarrisonedControllers.RemoveSwap(RequestAsController);
		return true;
	}

	return false;
}

bool UGarrisonComponent::HasDoors() const
{
	return Doors.Num() > 0;
}

USceneComponent& UGarrisonComponent::GetRandomDoor() const
{
#if !UE_BUILD_SHIPPING
	check(Doors.Num() > 0);
#endif //!UE_BUILD_SHIPPING

	return *Doors[FMath::RandRange(0, Doors.Num() - 1)];
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
