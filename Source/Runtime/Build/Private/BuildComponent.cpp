// Sowing Season - Tom Shinton 2020

#include "Runtime/Build/Public/BuildComponent.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include "Runtime/Build/Public/BuildInterface.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Ghost/BuildingGhost.h"
#include "Runtime/Build/Public/Ghost/GhostInterface.h"

#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>

DEFINE_LOG_CATEGORY_STATIC(BuildComponentLog, Log, Log)

UBuildComponent::UBuildComponent()
	: SoftGhostClass()
	, AsyncLoader(MakeUnique<FAsyncLoader>())
	, CurrentBuildData(nullptr)
	, GhostClass(nullptr)
	, CurrentGhost()
	, GridProjectionInterface(*this)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!SoftGhostClass.IsNull())
	{
		AsyncLoader->RequestLoad<UClass>(SoftGhostClass, [WeakThis = TWeakObjectPtr<UBuildComponent>(this), this](UClass& InLoadedClass)
		{
			if (WeakThis.IsValid())
			{
				if (InLoadedClass.ImplementsInterface(UGhostInterface::StaticClass()))
				{
					GhostClass = &InLoadedClass;
				}
			}
		});
	}

	if (IGridProjectionInterface* GridProjection = GridProjectionInterface.Get())
	{
		GridProjection->GetOnRoundedPositionChanged().AddLambda([WeakThis = TWeakObjectPtr<UBuildComponent>(this)](const FVector& InNewPosition, const FVector& InOldPosition)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->OnRoundedPositionChanged(InNewPosition, InOldPosition);
			}
		});
	}
}

void UBuildComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBuildComponent::StartBuildFromClass(const FSoftObjectPath& InBuildingData)
{
	if (!InBuildingData.IsNull())
	{
		AsyncLoader->RequestLoad<UBuildingData>(InBuildingData, [WeakThis = TWeakObjectPtr<UBuildComponent>(this), this](UBuildingData& LoadedBuildingData)
		{
			if (WeakThis.IsValid())
			{
				UE_LOG(BuildComponentLog, Log, TEXT("Loaded Building Data for %s"), *LoadedBuildingData.GetName());
				WeakThis->CurrentBuildData = &LoadedBuildingData;

				if (UWorld* World = GetWorld())
				{
					if (CurrentGhost.IsValid())
					{
						CurrentGhost->DestroyGhost();
					}

					if (GhostClass != nullptr)
					{
						if (AActor* NewGhost = World->SpawnActor<AActor>(GhostClass, FTransform::Identity))
						{
							CurrentGhost = *Cast<IGhostInterface>(NewGhost);
						}
					}
				}
			}
		});
	}
	else
	{
		TryCancelBuild();
	}
}

void UBuildComponent::TryCancelBuild()
{

}

void UBuildComponent::OnRoundedPositionChanged(const FVector& InNewPosition, const FVector& InOldPosition)
{
	UE_LOG(LogTemp, Log, TEXT("New rounded pos: %s from %s"), *InNewPosition.ToString(), *InOldPosition.ToString());
}
