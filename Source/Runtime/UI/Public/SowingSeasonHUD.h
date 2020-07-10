// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "SowingSeasonHUD.generated.h"

class URetainerBox;

UCLASS(abstract, Blueprintable)
class UI_API USowingSeasonHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	USowingSeasonHUD(const FObjectInitializer& ObjectInitializer);

protected:

	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	URetainerBox* SelectionContainer;
	
private:

	void InitialiseSelection();

	UPROPERTY()
	APlayerController* LocalController;
};
