// Sowing Season - Tom Shinton 2020

#include "Runtime/Build/Public/BuildComponent.h"

#include "Runtime/Build/Public/BuildingData/BuildingData.h"

#include "Runtime/Build/Public/BuildInterface.h"
#include "Runtime/Build/Public/BuildingData/BuildingData.h"
#include "Runtime/Build/Public/Events/BuildEvents.h"
#include "Runtime/Build/Public/Ghost/GhostRenderer.h"
#include "Runtime/Build/Public/Foundation/Foundation.h"
#include "Runtime/Build/Public/FoundationBuilder/FoundationBuilderFunctions.h"
#include "Runtime/Build/Public/FoundationBuilder/FoundationBuilder.h"

#include <ObjectMessaging/Public/Sender/ObjectMessagingFunctions.h>

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
	, CurrentFoundationBuilder(nullptr)
	, LastFoundation()
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

					CurrentFoundationBuilder = FoundationBuilderFunctions::GetBuilderForMode(*BuildingData, *this);

					if (CurrentFoundationBuilder != nullptr)
					{
						CurrentFoundationBuilder->Init(*BuildingData, [WeakThis](const FFoundation& InGeneratedFoundation)
						{
							if (WeakThis.IsValid())
							{
								WeakThis->OnFoundationGenerated(InGeneratedFoundation);
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
		if (CurrentFoundationBuilder != nullptr)
		{
			CurrentFoundationBuilder->StartBuild();
		}
	}
}

void UBuildComponent::EndBuild()
{
	if (BuildingData != nullptr)
	{
		if (CurrentFoundationBuilder != nullptr)
		{
			CurrentFoundationBuilder->EndBuild();

			ObjectMessagingFunctions::SendMessage<FBuildCompleteEvent>(*GetOwner()->GetGameInstance(), FBuildCompleteEvent(*BuildingData, LastFoundation));

			if (IsBuildingValid())
			{
				if (BuildingData->ShouldCancelBuildPostPlacement)
				{
					CancelBuild();
				}
			}

			GhostRenderer->ClearGhost();
		}
	}
}

void UBuildComponent::CancelBuild()
{
	if (BuildingData != nullptr)
	{
		if (CurrentFoundationBuilder != nullptr)
		{
			CurrentFoundationBuilder->Teardown();

			CurrentFoundationBuilder = nullptr;
			BuildingData = nullptr;
	
			GhostRenderer->ClearGhost();
		}
	}
}

void UBuildComponent::RotateBuild()
{
	if (CurrentFoundationBuilder != nullptr)
	{
		CurrentFoundationBuilder->RotateBuild();
	}
}

void UBuildComponent::OnFoundationGenerated(const FFoundation& InNewFoundation)
{
	GhostRenderer->UpdateRender(InNewFoundation);
	LastFoundation = InNewFoundation;
}

bool UBuildComponent::IsBuildingValid() const
{
	return true;
}
