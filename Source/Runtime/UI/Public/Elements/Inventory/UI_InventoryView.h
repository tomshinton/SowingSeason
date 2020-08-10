// Sowing Season - Tom Shinton 2020

#pragma once

#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include <Runtime/CoreUObject/Public/UObject/WeakInterfacePtr.h>
#include <Runtime/Inventory/Public/InventoryInterface.h>
#include <Runtime/Inventory/Public/Containers/InventoryItem.h>

#include "Runtime/UI/Public/Elements/Inventory/UI_InventoryItem.h"

#include "UI_InventoryView.generated.h"

class UVerticalBox;
class IInventoryInterface;
class UUI_InventoryItem;

UCLASS(abstract, Blueprintable, hidedropdown)
class UUI_InventoryView : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_InventoryView(const FObjectInitializer& ObjectInitialiser);

	void Initialise(IInventoryInterface& InInventory);
	void SynchronizeProperties() override;

protected:

	void NativeConstruct() override;
	void NativeDestruct() override;

	UPROPERTY(EditAnywhere, Category = "Design")
	TSubclassOf<UUI_InventoryItem> ItemViewClass;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ItemsBox;

	TWeakInterfacePtr<IInventoryInterface> Inventory;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Design")
	TArray<FInventoryItem> DebugItems;
#endif //WITH_EDITORONLY_DATA

private:

	void AddRow(const FInventoryItem& InItem);

	UFUNCTION()
	void UpdateItems();

	UPROPERTY()
	TArray<UUI_InventoryItem*> InventoryItems;
};
