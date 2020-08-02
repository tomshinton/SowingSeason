#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <Runtime/Inventory/Public/Containers/InventoryItem.h>

#include "UI_InventoryItem.generated.h"

class UHorizontalBox;
class UTextBlock;
struct FInventoryItem;

UCLASS(abstract, Blueprintable, hidedropdown)
class UUI_InventoryItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_InventoryItem(const FObjectInitializer& ObjectInitialiser);

	void SetItem(const FInventoryItem& InItem);

protected:


	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ItemBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

private:

	void Update();

	FInventoryItem LastItem;
};
