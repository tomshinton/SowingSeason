// Sowing Season - Tom Shinton 2020

#include "Runtime/UI/Public/SowingSeasonHUD.h"
#include "Runtime/UI/Public/Elements/Selection/UI_SelectionBox.h"

#include <Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <Runtime/Selection/Public/SelectionComponent.h>
#include <Runtime/UMG/Public/Components/RetainerBox.h>

USowingSeasonHUD::USowingSeasonHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, LocalController(nullptr)
{

}

void USowingSeasonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	LocalController = GetWorld()->GetFirstPlayerController();

	InitialiseSelection();
}

void USowingSeasonHUD::InitialiseSelection()
{
	if (UActorComponent* Component = LocalController->GetComponentByClass(USelectionComponent::StaticClass()))
	{
		TWeakObjectPtr<USowingSeasonHUD> WeakThis = TWeakObjectPtr<USowingSeasonHUD>(this);
		USelectionComponent* SelectionComponent = Cast<USelectionComponent>(Component);

		SelectionComponent->GetOnSelectDelegate().AddLambda([WeakThis](const TWeakInterfacePtr<ISelectionInterface>& InNewSelection)
		{
			if (WeakThis.IsValid())
			{
				if (UUI_SelectionBox* NewSelectionBox = CreateWidget<UUI_SelectionBox>(WeakThis.Get(), InNewSelection->GetSelectionBoxClass()))
				{
					NewSelectionBox->Initialise(InNewSelection);
					WeakThis->SelectionContainer->AddChild(NewSelectionBox);
				}
			}
		});

		SelectionComponent->GetOnDeselectDelegate().AddLambda([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->SelectionContainer->ClearChildren();
			}
		});
	}
}
