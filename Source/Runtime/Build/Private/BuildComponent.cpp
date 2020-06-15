// Sowing Season - Tom Shinton 2020

#include "Runtime/Build/Public/BuildComponent.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include "Runtime/Build/Public/BuildInterface.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Ghost/GhostRenderer.h"
#include "Runtime/Build/Public/PointBuilder/PointBuilderFunctions.h"

DEFINE_LOG_CATEGORY_STATIC(BuildComponentLog, Log, Log)

namespace BuildComponentBindings
{
	const FName StartBuildBinding = TEXT("StartBuild");
	const FName CancelBuildBinding = TEXT("CancelBuild");

	const FName RotateBuildingBinding = TEXT("RotateBuild");
}

UBuildComponent::UBuildComponent()
	: SoftGhostClass()
	, AsyncLoader(MakeUnique<FAsyncLoader>())
	, BuildingData(nullptr)
	, GhostClass(nullptr)
	, GhostRenderer()
	, CurrentPointBuilder(nullptr)
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

	PlayerInputComponent.BindAction(BuildComponentBindings::StartBuildBinding, IE_Pressed, this, &UBuildComponent::StartBuild);
	PlayerInputComponent.BindAction(BuildComponentBindings::StartBuildBinding, IE_Released, this, &UBuildComponent::EndBuild);
	PlayerInputComponent.BindAction(BuildComponentBindings::CancelBuildBinding, IE_Pressed, this, &UBuildComponent::CancelBuild);

	PlayerInputComponent.BindAction(BuildComponentBindings::RotateBuildingBinding, IE_Pressed, this, &UBuildComponent::RotateBuild);
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
				WeakThis->BuildingData = &LoadedBuildingData;
				
				if (GhostRenderer.IsValid())
				{
					GhostRenderer->SetGhostInfo(LoadedBuildingData);

					CurrentPointBuilder = PointBuilderFunctions::GetBuilderForMode(*BuildingData, *this);

					if (CurrentPointBuilder != nullptr)
					{
						CurrentPointBuilder->Init(*BuildingData, [WeakThis](const TArray<FFoundationPoint>& GeneratedPoints)
						{
							if (WeakThis.IsValid())
							{
								WeakThis->OnNewPointsGenerated(GeneratedPoints);
							}
						});
					}
				}
			}
		});
	}
	else
	{
		CancelBuild();
	}
}

void UBuildComponent::StartBuild()
{
	if (BuildingData != nullptr)
	{
		UE_LOG(BuildComponentLog, Log, TEXT("Starting build of %s, starting point generation"), *BuildingData->NameReadable);
	
		if (CurrentPointBuilder != nullptr)
		{
			CurrentPointBuilder->StartBuild();
		}
	}
}

void UBuildComponent::EndBuild()
{
	if (BuildingData != nullptr)
	{
		UE_LOG(BuildComponentLog, Log, TEXT("Ending build of %s, can proceed to spawn building actual"), *BuildingData->NameReadable);

		if (IsBuildingValid())
		{
			if (BuildingData->ShouldCancelBuildPostPlacement)
			{
				CancelBuild();
			}
		}
	}
}

void UBuildComponent::CancelBuild()
{
	if (BuildingData != nullptr)
	{
		UE_LOG(BuildComponentLog, Log, TEXT("Cancelling build of %s, building can be built again"), *BuildingData->NameReadable);

		if (CurrentPointBuilder != nullptr)
		{
			CurrentPointBuilder->Teardown();

			CurrentPointBuilder = nullptr;
			BuildingData = nullptr;
		}

		GhostRenderer->ClearGhost();
	}
}

void UBuildComponent::RotateBuild()
{
	if (CurrentPointBuilder != nullptr)
	{
		CurrentPointBuilder->RotateBuild();
	}
}

void UBuildComponent::OnNewPointsGenerated(const TArray<FFoundationPoint>& InNewPoints)
{
	GhostRenderer->UpdateRender(InNewPoints);
}

bool UBuildComponent::IsBuildingValid() const
{
	return true;
}
