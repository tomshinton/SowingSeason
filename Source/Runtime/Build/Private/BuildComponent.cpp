// Sowing Season - Tom Shinton 2020

#include "Runtime/Build/Public/BuildComponent.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include "Runtime/Build/Public/BuildInterface.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Ghost/GhostRenderer.h"

#include <Runtime/WorldGrid/Public/GridProjection/GridProjectionInterface.h>

DEFINE_LOG_CATEGORY_STATIC(BuildComponentLog, Log, Log)

UBuildComponent::UBuildComponent()
	: SoftGhostClass()
	, AsyncLoader(MakeUnique<FAsyncLoader>())
	, CurrentBuildData(nullptr)
	, GhostClass(nullptr)
	, GhostRenderer()
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
				if (InLoadedClass.ImplementsInterface(UGhostRendererInterface::StaticClass()))
				{
					WeakThis->InitialiseGhost(InLoadedClass);
				}
			}
		});
	}
}

void UBuildComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBuildComponent::SetupComponentInputBindings(UInputComponent& PlayerInputComponent)
{
	Super::SetupComponentInputBindings(PlayerInputComponent);
}

void UBuildComponent::InitialiseGhost(UClass& InGhostClass)
{
	if(UWorld * World = GetWorld())
	{
		if (AActor* Ghost = World->SpawnActor<AActor>(&InGhostClass, FTransform::Identity))
		{
			GhostRenderer = *Ghost->GetInterface<IGhostRendererInterface>();

#if WITH_EDITORONLY_DATA
			Ghost->SetFolderPath(TEXT("Renderer"));
#endif //WITH_EDITORONLY_DATA
		}
	}
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
				
				if (GhostRenderer.IsValid())
				{
					GhostRenderer->SetGhostInfo(LoadedBuildingData);
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