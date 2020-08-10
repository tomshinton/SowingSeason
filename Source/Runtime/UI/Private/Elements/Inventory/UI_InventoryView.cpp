// Sowing Season - Tom Shinton 2020

#include "Runtime/UI/Public/Elements/Inventory/UI_InventoryView.h"


#include <Runtime/UMG/Public/Components/VerticalBox.h>

UUI_InventoryView::UUI_InventoryView(const FObjectInitializer& ObjectInitialiser)
	: Super(ObjectInitialiser)
	, ItemsBox(nullptr)
	, Inventory()
{

}

void UUI_InventoryView::NativeConstruct()
{
	
}

void UUI_InventoryView::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (Inventory.IsValid())
	{
		Inventory->GetOnInventoryChanged().RemoveAll(this);
	}
}

void UUI_InventoryView::AddRow(const FInventoryItem& InItem)
{
	if (ItemsBox != nullptr && ItemViewClass != nullptr)
	{
		if (UUI_InventoryItem* NewItem = CreateWidget<UUI_InventoryItem>(this, ItemViewClass))
		{
			NewItem->SetItem(InItem);
			ItemsBox->AddChild(NewItem);
		}
	}
}

void UUI_InventoryView::UpdateItems()
{
	if (ItemsBox != nullptr && ItemViewClass != nullptr)
	{
		ItemsBox->ClearChildren();

		for (const FInventoryItem& Item : Inventory->GetItems())
		{
			AddRow(Item);
		}
	}
}

void UUI_InventoryView::Initialise(IInventoryInterface& InInventory)
{
	Inventory = InInventory;

	for (const FInventoryItem& Item : Inventory->GetItems())
	{
		AddRow(Item);
	}

	InInventory.GetOnInventoryChanged().AddUObject(this, &UUI_InventoryView::UpdateItems);
}

#if WITH_EDITORONLY_DATA
void UUI_InventoryView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (IsDesignTime())
	{
		if (ItemsBox != nullptr && ItemViewClass != nullptr)
		{
			ItemsBox->ClearChildren();

			for (const FInventoryItem& Item : DebugItems)
			{
				AddRow(Item);
			}
		}
	}
}
#endif //WITH_EDITORONLY_DATA
