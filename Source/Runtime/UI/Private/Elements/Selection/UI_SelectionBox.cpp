#include "Runtime/UI/Public/Elements/Selection/UI_SelectionBox.h"

#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <Runtime/UMG/Public/Components/CanvasPanel.h>
#include <Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include <Runtime/UMG/Public/Components/TextBlock.h>

UUI_SelectionBox::UUI_SelectionBox(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
{

}

void UUI_SelectionBox::NativeConstruct()
{
	Super::NativeConstruct();

	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));

	if (Selection.IsValid())
	{
		if (SelectionName != nullptr)
		{
			SelectionName->SetText(Selection->GetSelectionName());
		}
	}
}

void UUI_SelectionBox::Initialise(const TWeakInterfacePtr<ISelectionInterface>& InSelection)
{
	Selection = InSelection;

	LocalController = GetWorld()->GetFirstPlayerController();
	//Align before we add to viewport, so we dont see it snap for a frame
	AlignInViewport();
}

void UUI_SelectionBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	AlignInViewport();
}

void UUI_SelectionBox::AlignInViewport()
{
	if (LocalController != nullptr)
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(SelectionBox->Slot))
		{
			FVector2D NewLoc;
			UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(LocalController, Selection->GetSelectionBoxLocation(), NewLoc);

			CanvasPanelSlot->SetPosition(FVector2D(NewLoc.X, NewLoc.Y));
		}
	}
}
