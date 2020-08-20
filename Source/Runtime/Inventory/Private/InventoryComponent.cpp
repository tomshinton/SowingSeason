// Sowing Season - Tom Shinton 2020

#include "Runtime/Inventory/Public/InventoryComponent.h"

#include "Runtime/Inventory/Public/InventorySettings.h"

UInventoryComponent::UInventoryComponent()
	: InventorySettings(GetDefault<UInventorySettings>())
	, Inventory()
{

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.Initialise();

	for (const FInventoryItem& StartingItem : Inventory.StartingItems)
	{
		AddItem(StartingItem.Amount, StartingItem.Name);
	}
}

void UInventoryComponent::AddItem(const uint8 InNumToAdd, const FName& InItem)
{
	const FItemDataRow ItemData = UInventorySettings::GetItemInfo(InItem);

	uint8 NumLeft = InNumToAdd;
	for (FInventoryItem& Item : Inventory.Items)
	{
		if (Item.CanAcceptItem(InItem))
		{
			Item.TryAdd(NumLeft, InItem, ItemData);

			if (NumLeft <= 0)
			{
				break;
			}
		}
	}

	if (NumLeft != InNumToAdd)
	{
		OnInventoryChanged.Broadcast();
	}
}

void UInventoryComponent::RemoveItem(const uint8 InNumToRemove, const FName& InItem)
{
	//Loop backwards, heuristically makes most sense
	uint8 NumLeft = InNumToRemove;
	for (int8 i = Inventory.Items.Num() - 1; i >= 0; --i)
	{
		FInventoryItem& CurrentItem = Inventory.Items[i];
		if (CurrentItem.CanRemoveItem(InItem))
		{
			CurrentItem.TryRemove(NumLeft);

			if (NumLeft <= 0)
			{
				break;
			}
		}
	}

	if (NumLeft != InNumToRemove)
	{
		OnInventoryChanged.Broadcast();
	}
}

TArray<FInventoryItem> UInventoryComponent::GetItems() const
{
	return Inventory.Items;
}
