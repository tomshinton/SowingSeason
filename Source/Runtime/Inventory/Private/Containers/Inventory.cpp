// Sowing Season - Tom Shinton 2020

#include "Runtime/Inventory/Public/Containers/Inventory.h"

void FInventory::Initialise()
{
	checkf(Size > 0, TEXT("Cannot have an inventory of size 0"));
	Items.AddDefaulted(Size);
}
