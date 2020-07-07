// Sowing Season - Tom Shinton 2020

#include "SowingSeason/Public/SowingSeasonPlayerController.h"

#include <Runtime/Selection/Public/SelectionComponent.h>

#if !UE_BUILD_SHIPPING
#include "SowingSeason/Cheats/SowingSeasonCheatManager.h"
#endif //!UE_BUILD_SHIPPING

namespace SowingSeasonPlayerControllerPrivate
{
	const FName SelectionComponentName = TEXT("SelectionComponent");
}

ASowingSeasonPlayerController::ASowingSeasonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, SelectionComponent(ObjectInitializer.CreateDefaultSubobject<USelectionComponent>(this, SowingSeasonPlayerControllerPrivate::SelectionComponentName))
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

#if !UE_BUILD_SHIPPING
	CheatClass = USowingSeasonCheatManager::StaticClass();
#endif //!UE_BUILD_SHIPPING
}

void ASowingSeasonPlayerController::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING
	EnableCheats();
#endif //UE_BUILD_SHIPPING
	
	FInputModeGameAndUI InputMode;
	InputMode 
		.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways)
		.SetHideCursorDuringCapture(false)
		.SetWidgetToFocus(nullptr)
		.SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	
	SetInputMode(InputMode);
}
