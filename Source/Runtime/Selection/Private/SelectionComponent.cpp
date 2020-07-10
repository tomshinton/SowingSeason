#include "Runtime/Selection/Public/SelectionComponent.h"
#include "Runtime/Selection/Public/SelectionInterface.h"

#include <Runtime/Engine/Classes/GameFramework/PlayerController.h>

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarDrawDebugSelectionReadout(TEXT("Selection.DrawDebugSelectionReadout"), 0, TEXT("Draw to screen what we've currently got selected, and what we're hovering"));
#endif //!UE_BUILD_SHIPPING

namespace SelectionComponentPrivate
{
	const float UpdateRate = 1.f / 30.f; //About 30 times a second
	const float TraceDepth = 50000.f;
}

namespace SelectionComponentBindings
{
	const FName Select = TEXT("Select");
	const FName Deselect = TEXT("Deselect");
}

USelectionComponent::USelectionComponent()
	: SelectionChannel()
	, CachedWorld(nullptr)
	, OwningPlayerController(nullptr)
	, TraceHandle()
	, CurrentHoverInterface()
	, CurrentSelectionInterface()
{
	TraceDelegate.BindWeakLambda(this, [this](const FTraceHandle& Handle, FTraceDatum& Data)
	{
		//Reset handle
		TraceHandle._Data.FrameNumber = 0;

		OnTraceComplete(Data);
	});
}

void USelectionComponent::SetupComponentInputBindings(UInputComponent& PlayerInputComponent)
{
	Super::SetupComponentInputBindings(PlayerInputComponent);
	
	PlayerInputComponent.BindAction(SelectionComponentBindings::Select, IE_Pressed, this, &USelectionComponent::TrySelect);
	PlayerInputComponent.BindAction(SelectionComponentBindings::Deselect, IE_Pressed, this, &USelectionComponent::Deselect);
}

void USelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedWorld = GetWorld();
	CachedWorld->GetTimerManager().SetTimer(TraceTimerHandle, this, &USelectionComponent::QueryScene, SelectionComponentPrivate::UpdateRate, true, SelectionComponentPrivate::UpdateRate);

	OwningPlayerController = CastChecked<APlayerController>(GetOwner());
}

void USelectionComponent::QueryScene()
{
	if (TraceHandle._Data.FrameNumber == 0)
	{
		FVector CurrPosUnderMouse;
		FVector CurrDirUnderMouse;
		OwningPlayerController->DeprojectMousePositionToWorld(CurrPosUnderMouse, CurrDirUnderMouse);
		const FVector ProjectedEnd = CurrPosUnderMouse + (CurrDirUnderMouse * SelectionComponentPrivate::TraceDepth);

		FHitResult HitRes;
		TraceHandle = CachedWorld->AsyncLineTraceByChannel(EAsyncTraceType::Single, CurrPosUnderMouse, ProjectedEnd, SelectionChannel, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	}
}

void USelectionComponent::OnTraceComplete(const FTraceDatum& InTraceData)
{
	if (InTraceData.OutHits.Num() > 0)
	{
		const FHitResult CurrHit = InTraceData.OutHits[0];

		if (AActor* HitActor = CurrHit.Actor.Get())
		{
			if (ISelectionInterface* HitSelectionInterface = Cast<ISelectionInterface>(HitActor))
			{
				CurrentHoverInterface = *HitSelectionInterface;
			}
		}
	}
	else
	{
		CurrentHoverInterface = TWeakInterfacePtr<ISelectionInterface>();
	}

#if !UE_BUILD_SHIPPING
	if (CVarDrawDebugSelectionReadout.GetValueOnAnyThread())
	{
		const FString CurrentHoverName = CurrentHoverInterface.IsValid() ? CurrentHoverInterface->GetDebugName() : TEXT("Nothing");
		const FString CurrentSelectionName = CurrentSelectionInterface.IsValid() ? CurrentSelectionInterface->GetDebugName() : TEXT("Nothing");
		
		GEngine->AddOnScreenDebugMessage(1, SelectionComponentPrivate::UpdateRate + SMALL_NUMBER, FColor::White, FString::Printf(TEXT("Currently Hovering: %s"), *CurrentHoverName));
		GEngine->AddOnScreenDebugMessage(2, SelectionComponentPrivate::UpdateRate + SMALL_NUMBER, FColor::White, FString::Printf(TEXT("Currently Selecting: %s"), *CurrentSelectionName));
	}
#endif //!UE_BUILD_SHIPPING
}

void USelectionComponent::TrySelect()
{
	if (CurrentSelectionInterface != CurrentHoverInterface)
	{
		if (CurrentHoverInterface.IsValid())
		{
			Deselect();
			CurrentSelectionInterface = CurrentHoverInterface;
			CurrentSelectionInterface->OnSelected();

			if (OnSelect.IsBound())
			{
				OnSelect.Broadcast(CurrentSelectionInterface);
			}
		}
		else
		{
			Deselect();
		}
	}
}

void USelectionComponent::Deselect()
{
	if (CurrentSelectionInterface.IsValid())
	{
		CurrentSelectionInterface->OnUnselected();
		CurrentSelectionInterface = TWeakInterfacePtr<ISelectionInterface>();

		if (OnDeselect.IsBound())
		{
			OnDeselect.Broadcast();
		}
	}	
}
