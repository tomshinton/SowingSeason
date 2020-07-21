#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>
#include <Runtime/Selection/Public/SelectionInterface.h>

#include "UI_SelectionBox.generated.h"

class UCanvasPanel;
class UTextBlock;
class APlayerController;

UCLASS(abstract, Blueprintable, hidedropdown)
class UUI_SelectionBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_SelectionBox(const FObjectInitializer& ObjectInitialiser);

	void Initialise(const TWeakInterfacePtr<ISelectionInterface>& InSelection);

protected:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectionName;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SelectionBox;

	TWeakInterfacePtr<ISelectionInterface> Selection;

private:

	void AlignInViewport();

	UPROPERTY()
	APlayerController* LocalController;
};
